#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  BookWorm.move(200, 200); // spin both motors
  delay(1000); // for one entire second

  BookWorm.move(-200, -200); // now do the reverse
  delay(1000); // for one entire second
}
