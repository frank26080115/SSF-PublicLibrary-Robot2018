#include <BookWorm.h>

void setup() {
  // put your setup code here, to run once:
  BookWorm.begin(); // sets up the pin directions on the robot circuit

  BookWorm.disableAccelLimit(); // we do not need automatic acceleration limits for this project

  // since we need to see reflections from walls
  // we need to turn on the IR emitters
  BookWorm.setIrEmittersOn();

  // this is called a "while-loop"
  // we use it to say "do not start the code until the button is pressed"
  while (BookWorm.isButtonPressed() == false) {
    // do nothing but wait
  }
  delay(100); // a slight pause, for "debouncing"
  // wait until the button is released
  while (BookWorm.isButtonPressed()) {
    // do nothing but wait
  }
  delay(500); // a slight pause, also for "debouncing"
}

void loop() {
  // put your main code here, to run repeatedly:

  // read sensors and store them into variables
  int leftSensor = BookWorm.readSensorLeftSide();
  int rightSensor = BookWorm.readSensorRightSide();

  bool isLeft;

  bool shouldMoveForward = true;

  // print sensor readings to serial monitor
  Serial.print(leftSensor, DEC); Serial.print(", ");
  Serial.print(rightSensor, DEC); Serial.println();

  if (leftSensor > 30 || rightSensor > 30) // either sensor detected something
  {
    // remember if the wall was on the left or right
    if (leftSensor > rightSensor) {
      isLeft = true;
    }
    else {
      isLeft = false;
    }

    // this is called a "do-while-loop"
    // this will make the robot back away from the wall until it does not see a wall anymore
    do
    {
      BookWorm.move(-300, -300); // reverse
      delay(100); // for a short time
      // read the sensors again
      leftSensor = BookWorm.readSensorLeftSide();
      rightSensor = BookWorm.readSensorRightSide();
    }
    while (leftSensor > 30 || rightSensor > 30);

    // remember where the wall was before, turn away from it
    if (isLeft) {
      BookWorm.move(200, -200); // turn right
    }
    else {
      BookWorm.move(-200, 200); // turn left
    }
    delay(300 + BookWorm.readPotentiometer()); // turn for some time, we can adjust how long with the potentiometer
  }

  BookWorm.move(500, 500); // full speed ahead

  // if button is pressed, stop robot for a bit
  if (BookWorm.isButtonPressed())
  {
    BookWorm.move(0, 0); // stop robot
    delay(5000); // stop robot for 5 seconds
  }
}
