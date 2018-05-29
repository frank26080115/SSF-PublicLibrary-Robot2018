#include <Arduino.h>
#include "BookWorm.h"
#include "ContinuousServo.h"

extern ContinuousServo servoLeft;
extern ContinuousServo servoRight;

#ifndef usToTicks
#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#endif
#ifndef ticksToUs
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds
#endif
#ifndef TRIM_DURATION
#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 2009
#endif

#define SERVO_CENTER_TICKS    (usToTicks(1500 - TRIM_DURATION))

#define SERVO_ACCELERATION_LIMIT 4

static unsigned int servoDeadzoneLeft = 0;
static unsigned int servoDeadzoneRight = 0;
static signed int servoBiasLeft = 0;
static signed int servoBiasRight = 0;

void cBookWorm::move(signed int left, signed int right)
{
	moveLeftServo(left);
	moveRightServo(right);
}

void cBookWorm::moveLeftServo(signed int x)
{
	signed int ticks = SERVO_CENTER_TICKS;
	if (x > 0) {
		ticks += x;
		ticks += servoDeadzoneLeft;
		ContinuousServo_BothForward |= (1 << 1);
	}
	else if (x <= 0) {
		ticks += x;
		ticks -= servoDeadzoneLeft;
		ContinuousServo_BothForward &= ~(1 << 1);
	}
	ticks += servoBiasLeft;
	servoLeft.writeTicks(ticks);
}

void cBookWorm::moveRightServo(signed int x)
{
	signed int ticks = SERVO_CENTER_TICKS;
	x *= -1; // flip
	if (x > 0) {
		ticks += x;
		ticks += servoDeadzoneRight;
		ContinuousServo_BothForward |= (1 << 0);
	}
	else if (x <= 0) {
		ticks += x;
		ticks -= servoDeadzoneRight;
		ContinuousServo_BothForward &= ~(1 << 0);
	}
	ticks += servoBiasRight;
	servoRight.writeTicks(ticks);
}

#define BOOKWORM_MOVEMIXED_INPUT_LIMIT 1023
#define BOOKWORM_MOVEMIXED_OUTPUT_SCALE 0.3
void cBookWorm::moveMixed(signed int throttle, signed int steer)
{
	signed int left, right;
	calcMix(throttle, steer, &left, &right);
	move(left, right);
}

void cBookWorm::calcMix(signed int throttle, signed int steer, signed int * left, signed int * right)
{
	double throttled, steerd;
	double diff;
	double leftd, rightd;

	if (throttle > BOOKWORM_MOVEMIXED_INPUT_LIMIT) {
		throttle = BOOKWORM_MOVEMIXED_INPUT_LIMIT;
	}
	else if (throttle < -BOOKWORM_MOVEMIXED_INPUT_LIMIT) {
		throttle = -BOOKWORM_MOVEMIXED_INPUT_LIMIT;
	}

	if (steer > BOOKWORM_MOVEMIXED_INPUT_LIMIT) {
		steer = BOOKWORM_MOVEMIXED_INPUT_LIMIT;
	}
	else if (steer < -BOOKWORM_MOVEMIXED_INPUT_LIMIT) {
		steer = -BOOKWORM_MOVEMIXED_INPUT_LIMIT;
	}

	throttled = throttle;
	steerd = steer;

	leftd = -steerd;
	rightd = steerd;
	leftd += throttled;
	rightd += throttled;
	leftd *= BOOKWORM_MOVEMIXED_OUTPUT_SCALE;
	rightd *= BOOKWORM_MOVEMIXED_OUTPUT_SCALE;

	*left = (signed int)lround(leftd);
	*right = (signed int)lround(rightd);
}

void cBookWorm::setAccelLimit(unsigned int accel)
{
	servoLeft.setAccel(accel);
	servoRight.setAccel(-accel);
}

void cBookWorm::enableAccelLimit(void)
{
	setAccelLimit(SERVO_ACCELERATION_LIMIT);
}

void cBookWorm::disableAccelLimit(void)
{
	setAccelLimit(0);
}

void cBookWorm::setServoDeadzoneLeft(unsigned int x)
{
	servoDeadzoneLeft = x;
}

void cBookWorm::setServoDeadzoneRight(unsigned int x)
{
	servoDeadzoneRight = x;
}

void cBookWorm::setServoBiasLeft(signed int x)
{
	servoBiasLeft = x;
}

void cBookWorm::setServoBiasRight(signed int x)
{
	servoBiasRight = x;
}