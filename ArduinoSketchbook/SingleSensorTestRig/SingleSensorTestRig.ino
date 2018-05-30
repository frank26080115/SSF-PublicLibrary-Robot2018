#include <BookWorm.h>

/*
 * This sketch is made for a device Frank built to make sure that a sensor is not broken
 */

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin();
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x, y;
  BookWorm.setEmittersOff();
  delay(50);
  x = BookWorm.readSensorRightSide();
  BookWorm.setEmittersOn();
  delay(50);
  y = BookWorm.readSensorRightSide();
  if ((y - x) > 50)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
}
