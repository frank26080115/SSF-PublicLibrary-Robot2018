#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit
}

void loop() {
  // put your main code here, to run repeatedly:

  // read both side sensors, save them into variables
  int leftSensor = BookWorm.readSensorLeftFloor();
  int rightSensor = BookWorm.readSensorRightFloor();

  // the sensor readings are 0 to 1023,
  // the LED brightness values must be 0 to 255
  // even if the sensor is over a black surface, it will read a slightly high value
  // so we do a bit of math, subtract some from the reading, before dividing it
  int leftBrightness = (leftSensor - 500) / 2;
  int rightBrightness = (rightSensor - 500) / 2;

  // set the LED brightness
  BookWorm.setLedLeft(leftBrightness);
  BookWorm.setLedRight(rightBrightness);

  // debug output to serial monitor
  Serial.print(leftSensor, DEC); Serial.print(", ");
  Serial.print(rightSensor, DEC); Serial.print(", ");
  Serial.print(leftBrightness, DEC); Serial.print(", ");
  Serial.print(rightBrightness, DEC); Serial.print(", ");
  Serial.println(); // new line
}
