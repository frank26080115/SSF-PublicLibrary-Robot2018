#define pinButton 10
#define pinLedLeft 6
#define pinLedRight 5
#define pinEmitterLeft 3
#define pinEmitterRight 4
#define pinSensorLeftFloor A3
#define pinSensorLeftSide A2
#define pinSensorRightFloor A6
#define pinSensorRightSide A7

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(pinLedLeft, OUTPUT);
  pinMode(pinLedRight, OUTPUT);
  pinMode(pinEmitterLeft, OUTPUT);
  pinMode(pinEmitterRight, OUTPUT);

  pinMode(pinButton, INPUT);
  digitalWrite(pinButton, HIGH);

  digitalWrite(pinEmitterLeft, HIGH);
  digitalWrite(pinEmitterRight, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  int left;
  int right;

  if (digitalRead(pinButton) == LOW)
  {
    left = analogRead(pinSensorLeftSide);
    right = analogRead(pinSensorRightSide);
  }
  else
  {
    left = analogRead(pinSensorLeftFloor);
    right = analogRead(pinSensorRightFloor);
  }

  Serial.print(left, DEC);
  Serial.print(" , ");
  Serial.print(right, DEC);
  Serial.println();

  left = left / 4;
  right = right / 4;

  analogWrite(pinLedLeft, left);
  analogWrite(pinLedRight, right);
}
