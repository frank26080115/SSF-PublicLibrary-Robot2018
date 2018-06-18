static uint32_t neopixel_iterations = 0;
extern uint32_t command_buttonflags;

void neopixel_rainbow(uint32_t wait, bool reverse)
{
  static uint16_t j = 0;
  uint16_t i, k;
  neopixel_iterations++;
  if ((neopixel_iterations % wait) != 0 && wait > 0) {
    return;
  }

  for (i = 0; i < NUM_OF_NEOPIXELS; i++)
  {
    k = i;
    if (reverse) {
      k = NUM_OF_NEOPIXELS - i - 1;
    }
    neopixel.setPixelColor(k, ColourWheel(((i * 256 / NUM_OF_NEOPIXELS) + j) & 255));
  }
  neopixel.show();
  j++;
  j %= (256 * 5);
}

void neopixel_theaterchase(uint32_t wait, bool reverse)
{
  #define CHASE_SKIP 5
  static uint16_t j = 0, q = 0;
  int16_t i, k;
  neopixel_iterations++;
  if ((neopixel_iterations % wait) != 0 && wait > 0) {
    return;
  }

  for (i = 0; i < NUM_OF_NEOPIXELS; i += CHASE_SKIP)
  {
    k = i + q;
    if (reverse)
    {
      k = NUM_OF_NEOPIXELS - k - 1;
    }
    if (k < 0 || k >= NUM_OF_NEOPIXELS) {
      break;
    }
    neopixel.setPixelColor(k, ColourWheel((i + (j * 16)) % 255));    //turn every third pixel on
  }
  neopixel.show();
  for (i = 0; i < NUM_OF_NEOPIXELS; i += 3)
  {
    k = i + q;
    if (reverse)
    {
      k = NUM_OF_NEOPIXELS - k - 1;
    }
    if (k < 0 || k >= NUM_OF_NEOPIXELS) {
      break;
    }
    neopixel.setPixelColor(k, 0);    //turn every third pixel off
  }
  // do not call "show()", wait for next iteration
  q++;
  if ((q % CHASE_SKIP) == 0) {
    q = 0;
    j++;
  }
  if ((j % 256) == 0) {
    j = 0;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t ColourWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return neopixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return neopixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return neopixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void neopixelPatternTask()
{
  static bool lowBright = false; // this latch prevents .show() from being called too often
  // calling .show() too often results in failure in servo control and errors in received data
  // this is because .show() has to disable interrupts to ensure critical timing is met

  if ((command_buttonflags & (1 << 1)) != 0)
  {
    neopixel_rainbow(40, false);
    lowBright = false;
  }
  else if ((command_buttonflags & (1 << 2)) != 0)
  {
    neopixel_rainbow(40, true);
    lowBright = false;
  }
  else if ((command_buttonflags & (1 << 3)) != 0)
  {
    neopixel_theaterchase(500, false);
    lowBright = false;
  }
  else if ((command_buttonflags & (1 << 4)) != 0)
  {
    neopixel_theaterchase(500, true);
    lowBright = false;
  }
  else if (lowBright == false)
  {
    neopixel.setBrightness(8);
    neopixel.show();
    lowBright = true;;
  }
}

