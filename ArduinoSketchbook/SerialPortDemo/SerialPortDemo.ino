int count = 0;

void setup() {
  // put your setup code here, to run once:

  // initialize the serial port
  Serial.begin(9600);
  // this is different from BookWorm.begin()
  // we are not using any libraries in this demo
  // BookWorm.begin() would've done Serial.begin for you
}

void loop() {
  // put your main code here, to run repeatedly:

  // write the number onto the screen, with a new line
  Serial.println(count, DEC);

  // wait for one entire second
  delay(1000);

  // count up
  count = count + 1;
}
