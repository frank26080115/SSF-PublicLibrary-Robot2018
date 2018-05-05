#include <Arduino.h>
#include "BookWorm.h"
#include "BookWormIMU.h"
#include <MPU6050_6Axis_MotionApps20.h>
#include <helper_3dmath.h>
#include <avr/eeprom.h>
#include "checksum.h"

MPU6050 mpu;

cBookWormIMU BookWormIMU;

static bool hasInit = false;
static uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)

bool cBookWormIMU::init(bool forceCalib)
{
	uint8_t devStatus;
	if (hasInit != false)
	{
		if (forceCalib != false)
		{
			calibrate(forceCalib);
		}
		return true;
	}
	// join I2C bus (I2Cdev library doesn't do this automatically)
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
		PORTC &= ~(_BV(4) | _BV(5)); // turn off internal pull-up resistors on Arduino
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif

	mpu.initialize();
	if ((devStatus = mpu.dmpInitialize()) != 0)
	{
		printf("MPU init failed %d\r\n", devStatus);
		return false;
	}

	calibrate(forceCalib);

	mpu.setDMPEnabled(true);
	packetSize = mpu.dmpGetFIFOPacketSize();

	hasInit = true;
	return true;
}

double cBookWormIMU::getAngle(uint8_t idx)
{
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	static float ypr[3];    // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

	uint8_t mpuIntStatus = mpu.getIntStatus();

	// get current FIFO count
	uint16_t fifoCount = mpu.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) != 0 || fifoCount >= 1024)
	{
		// reset so we can continue cleanly
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));

	// otherwise, check for DMP data ready interrupt (this should happen frequently)
	}
	else if ((mpuIntStatus & 0x02) != 0)
	{
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize)
		{
			fifoCount = mpu.getFIFOCount();
		}

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		
		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
	}

	return ypr[idx] * 180.0 / M_PI;
}

/*
calibration code below is a refactoring of http://wired.chillibasket.com/2015/01/calibrating-mpu6050/
*/

#define BOOKWORM_IMUCALIB_MAGIC 0xAB
#define BOOKWORM_IMUCALIB_EEADDR 0x100
#define BOOKWORM_IMUCALIB_GYRODEADZONE 1
#define BOOKWORM_IMUCALIB_ACCELDEADZONE 8

typedef struct
{
	uint8_t magic;
	int ax_offset;
	int ay_offset;
	int az_offset;
	int gx_offset;
	int gy_offset;
	int gz_offset;
	uint16_t checksum;
}
mpu6050_calib_t;

typedef struct
{
	int mean_ax;
	int mean_ay;
	int mean_az;
	int mean_gx;
	int mean_gy;
	int mean_gz;
}
mpu6050_calibtemp_t;

static void imuMeanSensors(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib);
static void imuDoCalibration(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib);
static void imuReportCalibration(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib);

void cBookWormIMU::calibrate(bool force)
{
	mpu6050_calib_t calib;
	uint16_t checksum_calculated;
	if (hasInit == false)
	{
		Serial.print(F("Error: calibration started without initialization\r\n"));
		return;
	}
	eeprom_read_block(&calib, BOOKWORM_IMUCALIB_EEADDR, sizeof(mpu6050_calib_t));
	checksum_calculated = bookworm_fletcher16((uint8_t*)&calib, sizeof(mpu6050_calib_t) - sizeof(uint16_t));

	if (checksum_calculated != calib.checksum || calib.magic != BOOKWORM_IMUCALIB_MAGIC || force != false)
	{
		mpu6050_calibtemp_t calibtemp;
		mpu.setXAccelOffset(0);
		mpu.setYAccelOffset(0);
		mpu.setZAccelOffset(0);
		mpu.setXGyroOffset(0);
		mpu.setYGyroOffset(0);
		mpu.setZGyroOffset(0);
		imuMeanSensors(&calibtemp, &calib);
		imuDoCalibration(&calibtemp, &calib);
		imuReportCalibration(&calibtemp, &calib);
		calib.magic = BOOKWORM_IMUCALIB_MAGIC;
		calib.checksum = bookworm_fletcher16((uint8_t*)&calib, sizeof(mpu6050_calib_t) - sizeof(uint16_t));
		eeprom_update_block(&calib, BOOKWORM_IMUCALIB_EEADDR, sizeof(mpu6050_calib_t));
	}

	mpu.setXAccelOffset(calib.ax_offset);
	mpu.setYAccelOffset(calib.ay_offset);
	mpu.setZAccelOffset(calib.az_offset);
	mpu.setXGyroOffset(calib.gx_offset);
	mpu.setYGyroOffset(calib.gy_offset);
	mpu.setZGyroOffset(calib.gz_offset);
}

static void imuMeanSensors(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib)
{
	#define BOOKWORM_IMUCALIB_AVGCNT 1000
	#define BOOKWORM_IMUCALIB_AVGDISCARD 100
	int16_t ax, ay, az, gx, gy, gz;
	long i=0,sum_ax=0,sum_ay=0,sum_az=0,sum_gx=0,sum_gy=0,sum_gz=0;

	while (i < (BOOKWORM_IMUCALIB_AVGCNT + BOOKWORM_IMUCALIB_AVGDISCARD + 1))
	{
		// read raw accel/gyro measurements from device
		mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

		if (i > 100 && i <= (BOOKWORM_IMUCALIB_AVGCNT + BOOKWORM_IMUCALIB_AVGDISCARD))
		{
			sum_ax = sum_ax + ax;
			sum_ay = sum_ay + ay;
			sum_az = sum_az + az;
			sum_gx = sum_gx + gx;
			sum_gy = sum_gy + gy;
			sum_gz = sum_gz + gz;
		}
		if (i == (BOOKWORM_IMUCALIB_AVGCNT + BOOKWORM_IMUCALIB_AVGDISCARD))
		{
			calibtemp->mean_ax = sum_ax / BOOKWORM_IMUCALIB_AVGCNT;
			calibtemp->mean_ay = sum_ay / BOOKWORM_IMUCALIB_AVGCNT;
			calibtemp->mean_az = sum_az / BOOKWORM_IMUCALIB_AVGCNT;
			calibtemp->mean_gx = sum_gx / BOOKWORM_IMUCALIB_AVGCNT;
			calibtemp->mean_gy = sum_gy / BOOKWORM_IMUCALIB_AVGCNT;
			calibtemp->mean_gz = sum_gz / BOOKWORM_IMUCALIB_AVGCNT;
		}
		i++;
		delay(2); //Needed so we don't get repeated measures
	}
}

static void imuDoCalibration(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib)
{
	calib->ax_offset = -(calibtemp->mean_ax)/8;
	calib->ay_offset = -(calibtemp->mean_ay)/8;
	calib->az_offset = (0x4000-(calibtemp->mean_az))/8;

	calib->gx_offset = -(calibtemp->mean_gx)/4;
	calib->gy_offset = -(calibtemp->mean_gy)/4;
	calib->gz_offset = -(calibtemp->mean_gz)/4;

	while (1)
	{
		char ready = 0;
		mpu.setXAccelOffset(calib->ax_offset);
		mpu.setYAccelOffset(calib->ay_offset);
		mpu.setZAccelOffset(calib->az_offset);

		mpu.setXGyroOffset(calib->gx_offset);
		mpu.setYGyroOffset(calib->gy_offset);
		mpu.setZGyroOffset(calib->gz_offset);

		imuMeanSensors(calibtemp, calib);

		if (abs(calibtemp->mean_ax) <= BOOKWORM_IMUCALIB_ACCELDEADZONE) ready++;
		else calib->ax_offset = calib->ax_offset - calibtemp->mean_ax / BOOKWORM_IMUCALIB_ACCELDEADZONE;

		if (abs(calibtemp->mean_ay) <= BOOKWORM_IMUCALIB_ACCELDEADZONE) ready++;
		else calib->ay_offset = calib->ay_offset - calibtemp->mean_ay / BOOKWORM_IMUCALIB_ACCELDEADZONE;

		if (abs(0x4000 - calibtemp->mean_az) <= BOOKWORM_IMUCALIB_ACCELDEADZONE) ready++;
		else calib->az_offset = calib->az_offset + (0x4000 - calibtemp->mean_az) / BOOKWORM_IMUCALIB_ACCELDEADZONE;

		if (abs(calibtemp->mean_gx) <= BOOKWORM_IMUCALIB_GYRODEADZONE) ready++;
		else calib->gx_offset = calib->gx_offset - calibtemp->mean_gx / (BOOKWORM_IMUCALIB_GYRODEADZONE + 1);

		if (abs(calibtemp->mean_gy) <= BOOKWORM_IMUCALIB_GYRODEADZONE) ready++;
		else calib->gy_offset = calib->gy_offset - calibtemp->mean_gy / (BOOKWORM_IMUCALIB_GYRODEADZONE + 1);

		if (abs(calibtemp->mean_gz) <= BOOKWORM_IMUCALIB_GYRODEADZONE) ready++;
		else calib->gz_offset = calib->gz_offset - calibtemp->mean_gz / (BOOKWORM_IMUCALIB_GYRODEADZONE + 1);

		if (ready == 6) break;
	}
}

static void imuReportCalibration(mpu6050_calibtemp_t* calibtemp, mpu6050_calib_t* calib)
{
	Serial.print(F("MPU6050 Calib: "));
	Serial.print(calibtemp->mean_ax, DEC); Serial.print(F(", "));
	Serial.print(calibtemp->mean_ay, DEC); Serial.print(F(", "));
	Serial.print(calibtemp->mean_az, DEC); Serial.print(F(", "));
	Serial.print(calibtemp->mean_gx, DEC); Serial.print(F(", "));
	Serial.print(calibtemp->mean_gy, DEC); Serial.print(F(", "));
	Serial.print(calibtemp->mean_gz, DEC); Serial.print(F(", "));
	Serial.print(calib->ax_offset, DEC); Serial.print(F(", "));
	Serial.print(calib->ay_offset, DEC); Serial.print(F(", "));
	Serial.print(calib->az_offset, DEC); Serial.print(F(", "));
	Serial.print(calib->gx_offset, DEC); Serial.print(F(", "));
	Serial.print(calib->gy_offset, DEC); Serial.print(F(", "));
	Serial.print(calib->gz_offset, DEC); Serial.print(F("\r\n"));
}