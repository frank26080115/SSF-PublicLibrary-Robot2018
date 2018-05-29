/*
 * This is a simple program that shows you how to make the two LEDs blink without using any libraries
 * Here you will learn how to: 
 *  * define pin numbers
 *  * initialize pins as outputs
 *  * turn LEDs on and off
 *  * use a delay
 */

// here is how we tell Arduino that
// the left side LED is connected to pin 6
// and the right side LED is connected to pin 5
#define pinLedLeft 6
#define pinLedRight 5

void setup() {
  // put your setup code here, to run once:

  // here is how we make the pins output a signal
  pinMode(pinLedLeft, OUTPUT);
  pinMode(pinLedRight, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // here is how we turn on the LEDs
  digitalWrite(pinLedLeft, HIGH);
  digitalWrite(pinLedRight, HIGH);

  // here is how we use a delay function
  // without the delay, the LEDs will blink so fast that you won't even notice
  delay(500); // 500 means 500 milliseconds, which is half of a second

  // here is how we turn off the LEDs
  digitalWrite(pinLedLeft, LOW);
  digitalWrite(pinLedRight, LOW);

  // we have to use delay again
  delay(500);
}
