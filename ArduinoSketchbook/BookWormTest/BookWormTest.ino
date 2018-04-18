#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:

  BookWorm.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  //BookWorm.move(0, 0);
  BookWorm.move(BookWorm.readPotentiometer() - 512, BookWorm.readPotentiometer() - 512);

  Serial.println(BookWorm.readTvRemote());
}
