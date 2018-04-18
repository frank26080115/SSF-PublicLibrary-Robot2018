#ifndef _BOOKWORM_H_
#define _BOOKWORM_H_

#include <Arduino.h>
#include "BookWormTvRemote.h"
#include "ContinuousServo.h"
#include <avr/pgmspace.h>
#include "WString.h"

#define pinButton 10
#define pinLedLeft 6
#define pinLedRight 5
#define pinEmitterLeft 3
#define pinEmitterRight 4
#define pinSensorLeftFloor A3
#define pinSensorLeftSide A2
#define pinSensorRightFloor A6
#define pinSensorRightSide A7
#define pinPotentiometer A1
#define pinServoLeft 9 
#define pinServoRight 7
#define pinTvRemoteInput 8

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
	void setEmittersOn(void);
	void setEmittersOff(void);
	bool isButtonPressed(void);
	unsigned int readPotentiometer(void);
	unsigned int readSensorLeftSide(void);
	unsigned int readSensorLeftFloor(void);
	unsigned int readSensorRightSide(void);
	unsigned int readSensorRightFloor(void);

	void move(int left, int right);
	void moveLeftServo(int left);
	void moveRightServo(int right);
	void setAccelLimit(unsigned int accel);
	void enableAccelLimit(void);
	void disableAccelLimit(void);

	int readTvRemote(void);
	void sendTvRemote(uint8_t brand, unsigned long data, unsigned int nbits);

	bool initMpu6050(void);

	int printf(const char *format, ...);
	int printf(const __FlashStringHelper *format, ...);
};

extern cBookWorm BookWorm;

#endif
