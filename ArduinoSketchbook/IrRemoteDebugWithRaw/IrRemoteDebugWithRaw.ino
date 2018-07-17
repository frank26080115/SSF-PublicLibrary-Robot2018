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
    BookWorm.printf(F("IR Data Received! Brand: 0x%02X , Code: 0x%04X"), results.decode_type, results.value);
    BookWorm.printf(F(" , Bits: %d\r\n"), results.bits);

    // this function will print the raw timings of the pulses
    dumpRaw(&results);

    irrecv.resume(); // required if you need to read another command
  }
}

void dumpRaw(decode_results *results)
{
  // Print raw timing data
  Serial.print("Raw Timings[");
  Serial.print(results->rawlen-1, DEC); // total count
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) // for every item in the buffer
  {
    unsigned long  x = results->rawbuf[i] * USECPERTICK; // convert the time units from "ticks" to microseconds

    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }

    if (!(i % 8))  Serial.println(); // new line every 8 numbers, just so the screen looks neat
  }
  Serial.println(); // newline
  Serial.println(); // newline
}
