#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // read both side sensors, save them into variables
  int leftSensor = BookWorm.readSensorLeftFloor();
  int rightSensor = BookWorm.readSensorRightFloor();

  // the sensor readings are 0 to 1023,
  // but the LED brightness values must be 0 to 255
  // so we use math to divide the readings by 4
  int leftBrightness = (leftSensor - 500) / 2;
  int rightBrightness = (rightSensor - 500) / 2;

  // set the LED brightness
  BookWorm.setLedLeft(leftBrightness);
  BookWorm.setLedRight(rightBrightness);
}
