#include <Servo.h>

/*
 * this sketch is used to quickly verify that the servos are connected correctly, and are responding correctly
 * the potentiometer on the robot will determine the spin speed
 * use this sketch to help tune the center adjustment potentiometer of each servo
 */

#define pinLedLeft 6
#define pinLedRight 5
#define pinServoLeft 9 
#define pinServoRight 7
#define pinPotentiometer A1
#define pinButton 10

Servo servoLeft;
Servo servoRight;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  servoLeft.attach(pinServoLeft);
  servoRight.attach(pinServoRight);

  pinMode(pinLedLeft, OUTPUT);
  pinMode(pinLedRight, OUTPUT);

  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  int pot = analogRead(pinPotentiometer);

  Serial.println(pot, DEC);

  if (digitalRead(pinButton) == LOW)
  {
    digitalWrite(pinLedLeft, HIGH);
    digitalWrite(pinLedRight, HIGH);
    servoLeft.writeMicroseconds(1500 + (pot - 512));
    servoRight.writeMicroseconds(1500 + (pot - 512));
  }
  else
  {
    digitalWrite(pinLedLeft, LOW);
    digitalWrite(pinLedRight, LOW);
    servoLeft.writeMicroseconds(1500);
    servoRight.writeMicroseconds(1500);
  }
}
