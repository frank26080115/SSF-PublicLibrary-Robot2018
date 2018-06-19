
uint32_t command_pressedTime = 0;
uint32_t command_releasedTime = 0;
uint32_t command_buttonflags = 0;

void commandController()
{
  // we got a byte already indicating that the app is on the "controller" screen
  // read the rest of the packet
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT, true);

  if (len == 0) return; // do nothing if we got nothing
  // 0 also means error in packet

  // byte index [1] indicates what kind of control command is issued
  // here we only try to handle "colour" and "buttons"

  // colour
  if (packetbuffer[1] == 'C' && len == 6) {
    uint8_t red = packetbuffer[2];
    uint8_t green = packetbuffer[3];
    uint8_t blue = packetbuffer[4];
    uint8_t i;
    neopixel.updateLength(NUM_OF_NEOPIXELS);
    neopixel.setPin(NEOPIXEL_PIN);
    for (i = 0; i < NUM_OF_NEOPIXELS; i++) {
      neopixel.setPixelColor(i, red, green, blue);
    }
    neopixel.show();
    BookWorm.printf(F("Colour: 0x%02X%02X%02X\r\n"), red, green, blue);
  }

  // buttons
  if (packetbuffer[1] == 'B' && len == 5) {
    // the buttons are sent as numbers 1-8 in ASCII
    // the event type (pressed or released) is sent as 1 or 0 in ASCII
    // we can subtract '0' to do ASCII -> byte conversion
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    if (pressed)
    {
      command_pressedTime = millis(); // take a timestamp for timeout purposes
      command_buttonflags |= (1 << buttnum); // setting a bit flag indicates pressed
      if (buttnum <= 4) {
        neopixel.setBrightness(64);
      }
    }
    else
    {
      command_releasedTime = millis(); // take a timestamp for timeout purposes
      command_buttonflags &= ~(1 << buttnum); // clearing a bit flag indicates released
    }
    BookWorm.printf(F("BTN: 0x%08X\r\n"), command_buttonflags);
  }
}

/*
 * this function is called once per iteration of loop()
 * it is the function that issues the command to the servos according to what buttons are pressed
 * calling it frequenty allows us to implement a safety timeout so the robot doesn't run away when the connection is lost
 */
void driveTask()
{
  static bool isTimeout = false; // this static variable is meant to stop too many messages going out the serial port

  signed int throttle = 0, steering = 0;

  // implement a timeout just in case of a dropped bluetooth connection, so the robot doesn't run away
  if ((millis() - command_pressedTime) > 5000)
  {
    if (isTimeout == false) {
      BookWorm.printf(F("Press Timeout\r\n"));
    }
    command_buttonflags &= 0x1F;
    isTimeout = true;
  }

  // if the last event was a button release, we use a shorter timeout
  if ((millis() - command_releasedTime) > 500 && command_releasedTime > command_pressedTime)
  {
    if (isTimeout == false) {
      BookWorm.printf(F("Release Timeout\r\n"));
    }
    command_buttonflags &= 0x1F;
    isTimeout = true;
  }

  // determine which direction to go in depending on the button being pressed
  if ((command_buttonflags & (1 << 5)) != 0)
  {
    throttle += 1023;
  }
  if ((command_buttonflags & (1 << 6)) != 0)
  {
    throttle -= 1023;
  }
  if ((command_buttonflags & (1 << 8)) != 0)
  {
    steering += 800;
  }
  if ((command_buttonflags & (1 << 7)) != 0)
  {
    steering -= 800;
  }
  BookWorm.moveMixed(throttle, steering);
}

