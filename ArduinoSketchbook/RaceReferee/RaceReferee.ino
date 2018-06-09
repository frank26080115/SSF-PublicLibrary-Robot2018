#include <BookWorm.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // 16 column by 2 line LCD, with I2C address of 0x3F

#define pinLightSensor 0

// defining the states available in our state machine
// see switch-statement inside loop() later
enum
{
  STATEMACH_START_WAITING = 0,
  STATEMACH_START_PRIMED,
  STATEMACH_STARTED_UNSURE,
  STATEMACH_RUNNING,
  STATEMACH_ENDED,
};

void setup()
{
  BookWorm.begin();
  Wire.begin();

  PORTC &= ~(_BV(4) | _BV(5)); // prevent internal pull-up resistors from damaging anything

  lcd.init();
  PORTC &= ~(_BV(4) | _BV(5)); // prevent internal pull-up resistors from damaging anything, again, just in case

  lcd.backlight();

  lcd_bigNumInit(); // the big number clock trick requires loading custom characters into the LCD memory
  startMessage();
}

void loop()
{
  static int state = STATEMACH_START_WAITING;
  static uint32_t lastLcdTime = 0;
  static uint32_t startTime = 0;
  static uint32_t endTime = 0;
  uint32_t curMillis, dispMillis;
  bool seeLight;
  if (BookWorm.isButtonPressed())
  {
    state = STATEMACH_START_WAITING;
    startMessage();
    do
    {
      delay(200);
    }
    while (BookWorm.isButtonPressed());
  }

  curMillis = millis();
  seeLight = readLightSensor();

  /*
   * it's hard to explain state machines with just code comments
   * please read https://www.sparkfun.com/news/1801
   * which is an article about state machines, along with a simple example
   * 
   * also here's the article on switch-case statements
   * https://www.arduino.cc/reference/en/language/structure/control-structure/switchcase/
   */
  switch (state)
  {
    case STATEMACH_START_WAITING:
      if (readLightSensor() == false)
      {
        state = STATEMACH_START_PRIMED;
        lcd.setCursor(0, 1);
        lcd.print(F("Go! Go! Go! Go! "));
        lcd.setCursor(0, 0);
        lcd.print(F("                "));
      }
      break;
    case STATEMACH_START_PRIMED:
      
      if (seeLight != false)
      {
        state = STATEMACH_STARTED_UNSURE;
        startTime = curMillis;
      }
      break;
    case STATEMACH_STARTED_UNSURE:
      if (seeLight == false)
      {
        startTime = curMillis;
      }
      dispMillis = curMillis - startTime;
      if (dispMillis > 2000)
      {
        state = STATEMACH_RUNNING;
      }
      break;
    case STATEMACH_RUNNING:
      dispMillis = curMillis - startTime;
      if (seeLight == false)
      {
        endTime = curMillis;
        state = STATEMACH_ENDED;
      }
      break;
    case STATEMACH_ENDED:
      dispMillis = endTime - startTime;
      break;
  }

  curMillis = millis();
  if (state == STATEMACH_STARTED_UNSURE || state == STATEMACH_RUNNING || state == STATEMACH_ENDED)
  {
    if ((curMillis - lastLcdTime) >= 100) // only update the LCD when we need to
    {
      bool dots = state == STATEMACH_STARTED_UNSURE || state == STATEMACH_RUNNING;
      if (state == STATEMACH_ENDED)
      {
        // blink the dots if the race has finished
        // the code below is a bit of math that changes between true and false every half second
        dots = ((curMillis / 500) % 2) == 0;
      }

      lastLcdTime = curMillis;
      lcd_printTime(dispMillis, 1, dots);
    }
  }
}

bool readLightSensor()
{
  // the concept of hysteresis is best explained as a threshold that changes, so actually two thresholds, which one being used depends on the previous output state
  // this is used here to prevent any noise we get from the light sensor from confusing our readings when the light brightness transitions slowly
  #define LASER_HYSTERESIS 16
  static bool latched = false;
  
  int x = analogRead(pinLightSensor);
  int thresh = BookWorm.readPotentiometer();
  BookWorm.printf(F("light: %d ; pot: %d\r\n"), x, thresh); // show on computer screen

  // change latched output according to brightness, using hysteresis
  if (latched == false)
  {
    if (x < (thresh - (LASER_HYSTERESIS / 2)))
    {
      latched = true;
    }
  }
  else // latched == true
  {
    if (x > (thresh + (LASER_HYSTERESIS / 2)))
    {
      latched = false;
    }
  }

  // use LEDs to indicate status of sensor, without using hysteresis
  if (x > thresh)
  {
    BookWorm.setLedLeftOff();
    BookWorm.setLedRightOff();
  }
  else // latched == true
  {
    BookWorm.setLedLeftOn();
    BookWorm.setLedRightOn();
  }

  return latched;
}

void startMessage()
{
  lcd.setCursor(0, 0);
  lcd.print(F("Waiting to Start!"));
  lcd.setCursor(0, 1);
  lcd.print(F("                "));
}

