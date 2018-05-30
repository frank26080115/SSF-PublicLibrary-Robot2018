#include <BookWorm.h>
#include <IRremote_Include.h> // includes the IRremote library

IRrecv irrecv(pinTvRemoteInput); // declare instance with the pin number D8

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit

  // since we are using a TV remote
  // which is an IR light source
  // we can turn off our own IR light emitters
  BookWorm.setIrEmittersOff();

  // required to start IR reception
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:

  decode_results  results; // data structure variable

  if (irrecv.decode(&results)) // we have new remote command, saved into "results" data structure
  {
    // print the contents of the data structure to serial terminal
    BookWorm.printf(F("IR Data Received! Brand: 0x%02X , Code: 0x%04X , Bits: %d\r\n"), results.decode_type, results.value, results.bits);

    irrecv.resume(); // required if you need to read another command
  }
}
