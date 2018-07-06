#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit
  BookWorm.setServoStoppedNoPulse(true);
  BookWorm.disableAccelLimit();
}

void loop() {
  // put your main code here, to run repeatedly:

  int i;

  BookWorm.setLedBackOff();
  BookWorm.setLedRightOff();
  BookWorm.setLedLeftOff();
  BookWorm.move(0, 0);
  delay(2000);
  if (BookWorm.isButtonPressed() == 0 && getSpeed() > 0)
  {
    BookWorm.setLedBackOn();
    BookWorm.setLedRightOn();
    for (i = 0; i < 30; i++) {
      BookWorm.move(getSpeed(), getSpeed());
      delay(100);
    }
    BookWorm.setLedBackOff();
    BookWorm.setLedRightOff();
    BookWorm.move(0, 0);
    delay(2000);
    BookWorm.setLedBackOn();
    BookWorm.setLedLeftOn();
    for (i = 0; i < 30; i++) {
      BookWorm.move(-getSpeed(), -getSpeed());
      delay(100);
    }
  }

  Serial.println(getSpeed(), DEC);
}

int getSpeed() {
  int v = (BookWorm.readPotentiometer() / 2) - 100; // math is for range and deadband

  // limit output for deadband
  if (v < 0) {
    v = 0;
  }

  return v;
}

