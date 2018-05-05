#include <Arduino.h>
#include "BookWorm.h"
#include "ContinuousServo.h"

cBookWorm BookWorm;

ContinuousServo servoLeft;
ContinuousServo servoRight;

void cBookWorm::begin(void)
{
	Serial.begin(9600);
	UCSR0C |= _BV(USBS0); // use 2 stop bits instead of 1, for reliability purposes

	pinMode(pinLedLeft, OUTPUT);
	pinMode(pinLedRight, OUTPUT);
	pinMode(pinEmitterLeft, OUTPUT);
	pinMode(pinEmitterRight, OUTPUT);

	pinMode(pinButton, INPUT);
	digitalWrite(pinButton, HIGH);

	pinMode(pinTvRemoteInput, INPUT);
	digitalWrite(pinTvRemoteInput, HIGH);

	digitalWrite(pinEmitterLeft, HIGH);
	digitalWrite(pinEmitterRight, HIGH);

	servoLeft.attach(pinServoLeft);
	servoRight.attach(pinServoRight);
	move(0, 0);
	enableAccelLimit();
}

void cBookWorm::setLedLeft(int bright)
{
	bright = bright < 0 ? 0 : ( bright > 0xFF ? 0xFF : bright);
	analogWrite(pinLedLeft, bright);
}

void cBookWorm::setLedLeftOn(void)
{
	setLedLeft(0xFF);
}

void cBookWorm::setLedLeftOff(void)
{
	setLedLeft(0);
}

void cBookWorm::setLedRight(int bright)
{
	bright = bright < 0 ? 0 : ( bright > 0xFF ? 0xFF : bright);
	analogWrite(pinLedRight, bright);
}

void cBookWorm::setLedRightOn(void)
{
	setLedRight(0xFF);
}

void cBookWorm::setLedRightOff(void)
{
	setLedRight(0);
}

void cBookWorm::setLedBackOn(void)
{
	pinMode(pinLedBack, OUTPUT); // doing this here to avoid conflict if using SPI
	digitalWrite(pinLedBack, HIGH);
}

void cBookWorm::setLedBackOff(void)
{
	digitalWrite(pinLedBack, LOW);
}

void cBookWorm::setEmittersOn(void)
{
	pinMode(pinEmitterLeft, OUTPUT);
	pinMode(pinEmitterRight, OUTPUT);
	digitalWrite(pinEmitterLeft, HIGH);
	digitalWrite(pinEmitterRight, HIGH);
}

void cBookWorm::setEmittersOff(void)
{
	digitalWrite(pinEmitterLeft, LOW);
	digitalWrite(pinEmitterRight, LOW);
	pinMode(pinEmitterLeft, INPUT);
	pinMode(pinEmitterRight, INPUT);
}

bool cBookWorm::isButtonPressed(void)
{
	return digitalRead(pinButton) == LOW;
}

unsigned int cBookWorm::readPotentiometer(void)
{
	return analogRead(pinPotentiometer);
}

unsigned int cBookWorm::readSensorLeftSide(void)
{
	return analogRead(pinSensorLeftSide);
}

unsigned int cBookWorm::readSensorLeftFloor(void)
{
	return analogRead(pinSensorLeftFloor);
}

unsigned int cBookWorm::readSensorRightSide(void)
{
	return analogRead(pinSensorRightSide);
}

unsigned int cBookWorm::readSensorRightFloor(void)
{
	return analogRead(pinSensorRightFloor);
}