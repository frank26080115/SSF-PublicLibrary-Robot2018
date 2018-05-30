#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit
}

void loop() {
  // put your main code here, to run repeatedly:

  // light them up!
  BookWorm.setLedLeftOn();
  BookWorm.setLedRightOn();

  delay(500); // wait for half of a second

  // turn them off
  BookWorm.setLedLeftOff();
  BookWorm.setLedRightOff();

  delay(500); // wait for half of a second
}
