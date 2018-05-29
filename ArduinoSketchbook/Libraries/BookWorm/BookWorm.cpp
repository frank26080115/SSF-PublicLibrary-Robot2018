#include <Arduino.h>
#include "BookWorm.h"
#include "ContinuousServo.h"

cBookWorm BookWorm; // user accessible instance declared here

ContinuousServo servoLeft;  // not directly user accessible but accessible through library
ContinuousServo servoRight; // not directly user accessible but accessible through library

/*
Initializes all pins to default states, sets up the continuous rotation servos, enables serial port

return:	none
parameters:	none
*/
void cBookWorm::begin(void)
{
	Serial.begin(9600); // 9600 is slower but it's one less thing to screw up in the IDE
	UCSR0C |= _BV(USBS0); // use 2 stop bits instead of 1, for reliability purposes

	pinMode(pinLedLeft, OUTPUT);
	pinMode(pinLedRight, OUTPUT);

	pinMode(pinIrEmitterLeft, OUTPUT);
	pinMode(pinIrEmitterRight, OUTPUT);

	pinMode(pinButton, INPUT);
	digitalWrite(pinButton, HIGH);

	pinMode(pinTvRemoteInput, INPUT);
	digitalWrite(pinTvRemoteInput, HIGH);

	// default emitters ON so students don't wonder why there's no reading
	digitalWrite(pinIrEmitterLeft, HIGH);
	digitalWrite(pinIrEmitterRight, HIGH);

	servoLeft.attach(pinServoLeft);
	servoRight.attach(pinServoRight);
	move(0, 0);
	enableAccelLimit();
}

/*
Sets the brightness of the LED on the left side of the robot

return:	none
parameters:	bright: range 0 to 255, 0 meaning completely off, 255 meaning full brightness
*/
void cBookWorm::setLedLeft(int bright)
{
	bright = bright < 0 ? 0 : ( bright > 0xFF ? 0xFF : bright); // sanitise the input
	analogWrite(pinLedLeft, bright);
}

/*
Sets the brightness of the LED on the left side of the robot
to full brightness

return:	none
parameters:	none
*/
void cBookWorm::setLedLeftOn(void)
{
	setLedLeft(0xFF);
}

/*
Turns off the LED on the left side of the robot

return:	none
parameters:	none
*/
void cBookWorm::setLedLeftOff(void)
{
	setLedLeft(0);
}

/*
Sets the brightness of the LED on the right side of the robot

return:	none
parameters:	bright: range 0 to 255, 0 meaning completely off, 255 meaning full brightness
*/
void cBookWorm::setLedRight(int bright)
{
	bright = bright < 0 ? 0 : ( bright > 0xFF ? 0xFF : bright);
	analogWrite(pinLedRight, bright);
}

/*
Sets the brightness of the LED on the right side of the robot
to full brightness

return:	none
parameters:	none
*/
void cBookWorm::setLedRightOn(void)
{
	setLedRight(0xFF);
}

/*
Turns off the LED on the right side of the robot

return:	none
parameters:	none
*/
void cBookWorm::setLedRightOff(void)
{
	setLedRight(0);
}

/*
Turns on the LED on the top of the Arduino Nano (pin 13)

return:	none
parameters:	none
*/
void cBookWorm::setLedBackOn(void)
{
	pinMode(pinLedBack, OUTPUT); // doing this here to avoid conflict if using SPI
	digitalWrite(pinLedBack, HIGH);
}

/*
Turns off the LED on the top of the Arduino Nano (pin 13)

return:	none
parameters:	none
*/
void cBookWorm::setLedBackOff(void)
{
	digitalWrite(pinLedBack, LOW);
}

/*
Turns on all of the infrared emitters on the left and right side of the robot

return:	none
parameters:	none
*/
void cBookWorm::setIrEmittersOn(void)
{
	pinMode(pinIrEmitterLeft, OUTPUT);
	pinMode(pinIrEmitterRight, OUTPUT);
	digitalWrite(pinIrEmitterLeft, HIGH);
	digitalWrite(pinIrEmitterRight, HIGH);
	delay(1); // wait to ensure full brightness
}

/*
Turns off all of the infrared emitters on the left and right side of the robot

return:	none
parameters:	none
*/
void cBookWorm::setIrEmittersOff(void)
{
	digitalWrite(pinIrEmitterLeft, LOW);
	digitalWrite(pinIrEmitterRight, LOW);
	pinMode(pinIrEmitterLeft, INPUT);
	pinMode(pinIrEmitterRight, INPUT);
	delay(1); // wait to ensure fully off
}

/*
Reads the state of the button switch

return:	boolean, true if the button is pressed, false if the button is released
parameters:	none
*/
bool cBookWorm::isButtonPressed(void)
{
	return digitalRead(pinButton) == LOW;
}

/*
Reads the analog reading of the potentiometer

return:	unsigned 10 bit integer, range 0 to 1023,
		0 meaning rotated all the way down (counterclockwise),
		1023 meaning rotated all the way up (clockwise)

parameters:	none
*/
unsigned int cBookWorm::readPotentiometer(void)
{
	return analogRead(pinPotentiometer);
}

/*
Reads the analog reading of the left side infrared phototransistor

return:	unsigned 10 bit integer, range 0 to 1023, 0 meaning dark, 1023 meaning maximum brightness
parameters:	none
*/
unsigned int cBookWorm::readSensorLeftSide(void)
{
	return analogRead(pinSensorLeftSide);
}

/*
Reads the analog reading of the left floor infrared phototransistor

return:	unsigned 10 bit integer, range 0 to 1023, 0 meaning dark, 1023 meaning maximum brightness
parameters:	none
*/
unsigned int cBookWorm::readSensorLeftFloor(void)
{
	return analogRead(pinSensorLeftFloor);
}

/*
Reads the analog reading of the right side infrared phototransistor

return:	unsigned 10 bit integer, range 0 to 1023, 0 meaning dark, 1023 meaning maximum brightness
parameters:	none
*/
unsigned int cBookWorm::readSensorRightSide(void)
{
	return analogRead(pinSensorRightSide);
}

/*
Reads the analog reading of the right floor infrared phototransistor

return:	unsigned 10 bit integer, range 0 to 1023, 0 meaning dark, 1023 meaning maximum brightness
parameters:	none
*/
unsigned int cBookWorm::readSensorRightFloor(void)
{
	return analogRead(pinSensorRightFloor);
}

/*
Moves both left and right continuous rotation servos at a given speed
Direction for the servos are corrected for you

return: none
parameters:
				left: left side servo speed, value between -500 and +500, 0 meaning stopped, +500 means full speed forward, -500 means full speed reverse
				right: right side servo speed, value between -500 and +500, 0 meaning stopped, +500 means full speed forward, -500 means full speed reverse
*/
// void cBookWorm::move(signed int left, signed int right);
/* this function actually is written inside BookWormMove.cpp but I've put the documentation here for quicker access */