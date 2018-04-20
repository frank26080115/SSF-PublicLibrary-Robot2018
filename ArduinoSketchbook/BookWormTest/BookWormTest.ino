#include <BookWorm.h>

uint32_t timeStamp = 0;
int curCmd = TVREMOTE_KEY_NONE;

int leftSensMax = 0;
int rightSensMax = 0;
#define SENS_MAX_DECAY 1

bool lineFollow = false;

void setup() {
  // put your setup code here, to run once:

  BookWorm.begin();
  BookWorm.setEmittersOff();
}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t now = millis();
  int rmtRes = BookWorm.readTvRemote();
  if (rmtRes != TVREMOTE_KEY_NONE && rmtRes != TVREMOTE_KEY_UNKNOWN)
  {
    timeStamp = now;
    //if (rmtRes != TVREMOTE_KEY_REPEATLAST)
    {
      curCmd = rmtRes;
    }
    if (rmtRes == TVREMOTE_KEY_PLAY)
    {
      BookWorm.enableAccelLimit();
      lineFollow = true;
    }
    else
    {
      BookWorm.disableAccelLimit();
      lineFollow = false;
    }
  }

  if ((now - timeStamp) > 200)
  {
    curCmd = TVREMOTE_KEY_NONE;
    if (lineFollow == false)
    {
      BookWorm.disableAccelLimit();
      BookWorm.move(0, 0);
    }
  }

  int pot = BookWorm.readPotentiometer();
  int leftSens = BookWorm.readSensorLeftSide();
  int rightSens = BookWorm.readSensorRightSide();
  if (lineFollow == false)
  {
    BookWorm.setLedLeft(leftSensMax / 4);
    BookWorm.setLedRight(rightSensMax / 4);
  }

  if (leftSens > leftSensMax)
  {
    leftSensMax = leftSens;
  }
  else if (leftSensMax >= SENS_MAX_DECAY)
  {
    leftSensMax -= SENS_MAX_DECAY;
  }

  if (rightSens > rightSensMax)
  {
    rightSensMax = rightSens;
  }
  else if (rightSensMax >= SENS_MAX_DECAY)
  {
    rightSensMax -= SENS_MAX_DECAY;
  }

  if (curCmd == TVREMOTE_KEY_SELECT && (rightSensMax > 16 || leftSensMax > 16))
  {
    int32_t diff = abs(rightSensMax - leftSensMax);

    if (leftSensMax > rightSensMax)
    {
      BookWorm.move(500 - (diff * 2), 500);
    }
    else
    {
      BookWorm.move(500, 500 - (diff * 2));
    }
  }
  else if (curCmd == TVREMOTE_KEY_UP)
  {
    BookWorm.move(500, 500);
  }
  else if (curCmd == TVREMOTE_KEY_DOWN)
  {
    BookWorm.move(-200, -200);
  }
  else if (curCmd == TVREMOTE_KEY_LEFT)
  {
    BookWorm.move(-200, 200);
  }
  else if (curCmd == TVREMOTE_KEY_RIGHT)
  {
    BookWorm.move(200, -200);
  }

  if (lineFollow || BookWorm.isButtonPressed())
  {
    leftSens = BookWorm.readSensorLeftFloor();
    rightSens = BookWorm.readSensorRightFloor();
    BookWorm.setLedLeft(leftSens / 4);
    BookWorm.setLedRight(rightSens / 4);
    if (((now / 100) % 2) == 0)
    {
      if (leftSens < pot)
      {
        BookWorm.setLedLeftOff();
      }
      if (rightSens < pot)
      {
        BookWorm.setLedRightOff();
      }
    }

    if (BookWorm.isButtonPressed())
    {
      delay(100);
    }

    if (lineFollow)
    {
      if (leftSens >= pot && rightSens < pot)
      {
        BookWorm.move(200, 0);
      }
      else if (leftSens < pot && rightSens >= pot)
      {
        BookWorm.move(0, 200);
      }
      else if (leftSens >= pot && rightSens >= pot)
      {
        BookWorm.move(500, 500);
      }
      else if (leftSens < pot && rightSens < pot)
      {
        BookWorm.move(200, 200);
      }
    }
  }
}
