#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit
}

void loop() {
  // put your main code here, to run repeatedly:

  // read both side sensors, save them into variables
  int leftSensor = BookWorm.readSensorLeftSide();
  int rightSensor = BookWorm.readSensorRightSide();

  // the sensor readings are 0 to 1023,
  // but the LED brightness values must be 0 to 255
  // so we use math to divide the readings by 4
  int leftBrightness = leftSensor / 4;
  int rightBrightness = rightSensor / 4;

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
