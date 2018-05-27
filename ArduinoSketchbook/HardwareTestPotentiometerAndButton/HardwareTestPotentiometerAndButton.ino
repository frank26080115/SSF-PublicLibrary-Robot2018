#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // read potentiometer value, save it into a variable
  int pot = BookWorm.readPotentiometer();

  // the potentiometer value reading is 0 to 1023,
  // but the LED brightness values must be 0 to 255
  // so we use math to divide the readings by 4
  int leftBrightness = pot / 4;

  // set the LED brightness
  BookWorm.setLedLeft(leftBrightness);

  // debug output to serial monitor
  Serial.print(pot, DEC); Serial.print(", ");

  if (BookWorm.isButtonPressed()) // determine if the button is pressed
  {
    // if it is pressed
    BookWorm.setLedRightOn();

    // debug output to serial monitor
    Serial.print("button pressed");
  }
  else
  {
    // if it is not pressed
    BookWorm.setLedRightOff();

    // debug output to serial monitor
    Serial.print("button released");
  }

  Serial.println(); // new line
}
