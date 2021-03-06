#ifndef _BOOKWORM_H_
#define _BOOKWORM_H_

#include <Arduino.h>
#include "BookWormTvRemote.h"
#include "ContinuousServo.h"
#include <avr/pgmspace.h>
#include "WString.h"

// pin definitions that can be utilized by other libraries
#define pinButton           10
#define pinLedLeft           6
#define pinLedRight          5
#define pinLedBack          13
#define pinIrEmitterLeft     3
#define pinIrEmitterRight    4
#define pinSensorLeftFloor  A3
#define pinSensorLeftSide   A2
#define pinSensorRightFloor A6
#define pinSensorRightSide  A7
#define pinPotentiometer    A1
#define pinServoLeft         9
#define pinServoRight        7
#define pinTvRemoteInput     8

class __FlashStringHelper;

class cBookWorm
{
public:
	void begin(void);
	void setLedLeft(int bright);
	void setLedLeftOn(void);
	void setLedLeftOff(void);
	void setLedRight(int bright);
	void setLedRightOn(void);
	void setLedRightOff(void);
	void setLedBackOn(void);
	void setLedBackOff(void);
	void setIrEmittersOn(void);
	void setIrEmittersOff(void);
	bool isButtonPressed(void);
	unsigned int readPotentiometer(void);
	unsigned int readSensorLeftSide(void);
	unsigned int readSensorLeftFloor(void);
	unsigned int readSensorRightSide(void);
	unsigned int readSensorRightFloor(void);

	void move(signed int left, signed int right);
	void moveLeftServo(signed int left);
	void moveRightServo(signed int right);
	void moveMixed(signed int throttle, signed int steer);

	// these functions below require more understanding of how servo signals work
	void calcMix(signed int throttle, signed int steer, signed int * left, signed int * right);
	void setAccelLimit(unsigned int accel);
	void enableAccelLimit(void);
	void disableAccelLimit(void);
	void setServoDeadzoneLeft(unsigned int x);
	void setServoDeadzoneRight(unsigned int x);
	void setServoBiasLeft(signed int x);
	void setServoBiasRight(signed int x);
	void setServoStoppedNoPulse(bool);

	void initTvRemote(void);

	// these functions require the user to modify the library
	int readTvRemote(void);
	void sendTvRemote(uint8_t brand, unsigned long data, unsigned int nbits);

	// these are for making it easier to debug
	int printf(const char *format, ...);
	int printf(const __FlashStringHelper *format, ...);
};

extern cBookWorm BookWorm; // declare user accessible instance

#endif
