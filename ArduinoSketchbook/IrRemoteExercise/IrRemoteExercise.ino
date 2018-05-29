#include <BookWorm.h>
#include <IRremote_Include.h> // includes the IRremote library

IRrecv irrecv(pinTvRemoteInput); // declare instance with the pin number D8

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin();

  // since we are using a TV remote
  // which is an IR light source
  // we can turn off our own IR light emitters
  BookWorm.setIrEmittersOff();

  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:

  decode_results  results; // data structure variable

  if (irrecv.decode(&results)) // we have new remote command, saved into "results" data structure
  {
    if (results.decode_type == 0x04 && results.value == 0x0090) // chan-up is pressed
    {
      BookWorm.move(300, 300); // move forward
      delay(500);
    }
    else if (results.decode_type == 0x04 && results.value == 0x0890) // chan-down is pressed
    {
      BookWorm.move(-300, -300); // move backward
      delay(500);
    }
    else // something detected but we do not understand
    {
      BookWorm.move(0, 0); // stop robot
    }

    irrecv.resume(); // required if you need to read another command
  }
  else // no new IR remote command
  {
    BookWorm.move(0, 0); // stop robot
  }
}
