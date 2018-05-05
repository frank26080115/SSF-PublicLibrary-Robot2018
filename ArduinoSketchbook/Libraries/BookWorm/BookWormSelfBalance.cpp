#include "BookWorm.h"
#include "BookWormSelfBalance.h"

#include "ContinuousServo.h"
#include <PID_v1.h>
#include <helper_3dmath.h>

extern ContinuousServo servoLeft;
extern ContinuousServo servoRight;

static double input, output;
static double setpoint;
static double Kp = 50;
static double Kd = 1.4;
static double Ki = 60;
static PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

bool cBookWormSelfBalance::init(void)
{
	setpoint = 180.0;
	pid.SetMode(AUTOMATIC);
	pid.SetSampleTime(10);
	pid.SetOutputLimits(-255, 255);  
}

void cBookWormSelfBalance::selfBalance(signed int throttle, signed int steer)
{
	
}