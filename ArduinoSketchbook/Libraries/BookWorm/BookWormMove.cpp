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

#define SERVO_CENTER_TICKS    usToTicks(1500 - TRIM_DURATION)

#define SERVO_ACCELERATION_LIMIT 4

void cBookWorm::move(int left, int right)
{
	int leftTicks = SERVO_CENTER_TICKS + left;
	int rightTicks = SERVO_CENTER_TICKS - right;
	printf(F("%u, %u,\r\n"), leftTicks, rightTicks);
	servoLeft.writeTicks(leftTicks);
	servoRight.writeTicks(rightTicks);
}

void cBookWorm::moveLeftServo(int left)
{
	int leftTicks = SERVO_CENTER_TICKS + left;
	servoLeft.writeTicks(leftTicks);
}

void cBookWorm::moveRightServo(int right)
{
	int rightTicks = SERVO_CENTER_TICKS - right;
	servoRight.writeTicks(rightTicks);
}

void cBookWorm::setAccelLimit(unsigned int accel)
{
	servoLeft.setAccel(accel);
	servoLeft.setAccel(-accel);
}

void cBookWorm::enableAccelLimit(void)
{
	setAccelLimit(SERVO_ACCELERATION_LIMIT);
}

void cBookWorm::disableAccelLimit(void)
{
	setAccelLimit(0);
}