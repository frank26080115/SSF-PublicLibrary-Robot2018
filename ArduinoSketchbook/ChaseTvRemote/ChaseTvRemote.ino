#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit

  // since we are chasing a TV remote
  // which is an IR light source
  // we can turn off our own IR light emitters
  BookWorm.setIrEmittersOff();
}

void loop() {
  // put your main code here, to run repeatedly:

  // read sensors and store them into variables
  int leftSensor = BookWorm.readSensorLeftSide();
  int rightSensor = BookWorm.readSensorRightSide();

  // print sensor readings to serial monitor
  Serial.print(leftSensor, DEC); Serial.print(", ");
  Serial.print(rightSensor, DEC); Serial.println();

  // check where the object is
  if (leftSensor > 30 || rightSensor > 30) // either sensor detected something
  {
    if (leftSensor > rightSensor) // more IR light from the left side
    {
      BookWorm.move(120, 300); // turn left
      delay(500);
    }
    else // more IR light from the right side
    {
      BookWorm.move(300, 120); // turn right
      delay(500);
    }
  }
  else // not enough IR light detected
  {
    BookWorm.move(0, 0); // stop
  }
}
