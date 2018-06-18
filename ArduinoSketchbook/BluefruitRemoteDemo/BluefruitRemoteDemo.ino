#include <BookWorm.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "BluefruitConfig.h"
#include <Adafruit_NeoPixel.h>
#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

#define NEOPIXEL_PIN            13
#define NUM_OF_NEOPIXELS        24
#define ENABLE_ADVANCED_NEOPIXEL_CONTROL

/*
 * Command mode allows for more advanced functions such as
 *  - resetting the Bluefruit module
 *  - checking if there is an established connection
 * 
 * But, command mode uses more wires
 * so if you want to connect more things to your circuit...
 * you should disable command mode by commenting-out the definition below
 */
//#define USE_COMMAND_MODE

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout, bool skip);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
extern uint8_t packetbuffer[];

// Create the bluefruit object
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluefruitSS,
  BLUEFRUIT_UART_MODE_PIN,
  BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

// Create the neopixel object
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(NUM_OF_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  BookWorm.begin();
  #ifndef USE_COMMAND_MODE
  bluefruitSS.begin(9600);
  bluefruitSS.setTimeout(BLE_DEFAULT_TIMEOUT);
  if (BLUEFRUIT_UART_MODE_PIN >= 0) {
    pinMode(BLUEFRUIT_UART_MODE_PIN, OUTPUT);
    digitalWrite(BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_MODE_DATA);
  }
  if (BLUEFRUIT_UART_RTS_PIN >= 0) {
    pinMode(BLUEFRUIT_UART_RTS_PIN, INPUT);
  }
  if (BLUEFRUIT_UART_CTS_PIN >= 0) {
    pinMode(BLUEFRUIT_UART_CTS_PIN, OUTPUT);
    digitalWrite(BLUEFRUIT_UART_CTS_PIN, LOW);
  }
  #else
  if ( !ble.begin(VERBOSE_MODE) )
  {
    error_halt(F("Couldn't find Bluefruit, check wiring?"));
  }
  Serial.println( F("Bluefruit initialized") );
  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error_halt(F("Couldn't factory reset"));
    }
  }
  /* Disable command echo from Bluefruit */
  ble.echo(false);
  Serial.println( F("Requesting Bluefruit info:") );
  /* Print Bluefruit information */
  ble.info();
  /* Avoid annoying output */
  ble.verbose(false);
  #endif

  neopixel.begin();
}

void loop()
{
  static bool conn = false;

  driveTask();
  neopixelPatternTask();

#ifdef USE_COMMAND_MODE
  if (conn == false)
  {
    if (! ble.isConnected())
    {
      conn = false;
      // double fast blink LEDs shows not connected
      BookWorm.setLedLeftOn();
      BookWorm.setLedRightOn();
      delay(100);
      BookWorm.setLedLeftOff();
      BookWorm.setLedRightOff();
      delay(100);
      BookWorm.setLedLeftOn();
      BookWorm.setLedRightOn();
      delay(100);
      BookWorm.setLedLeftOff();
      BookWorm.setLedRightOff();
      delay(500);
      return;
    }
    Serial.println( F("Bluetooth Connected") );
    ble.setMode(BLUEFRUIT_MODE_DATA);
  }
#endif

  conn = true;
  
  // both LEDs on means connected
  BookWorm.setLedLeftOn();
  BookWorm.setLedRightOn();

#ifdef USE_COMMAND_MODE
  if (ble.available() == 0) {
    return;
  }
  #else
  if (BLUEFRUIT_UART_CTS_PIN >= 0) {
    digitalWrite(BLUEFRUIT_UART_CTS_PIN, HIGH);
    digitalWrite(BLUEFRUIT_UART_CTS_PIN, LOW);
  }
  if (bluefruitSS.available() == 0) {
    return;
  }
#endif

  int command = ble.read();

  switch (command)
  {
    #ifndef USE_COMMAND_MODE
    #ifdef ENABLE_ADVANCED_NEOPIXEL_CONTROL
    case 'V': {   // Get Version
        commandVersion();
        break;
      }

    case 'S': {   // Setup dimensions, components, stride...
        commandSetup();
        break;
     }

    case 'C': {   // Clear with color
        commandClearColor();
        break;
    }

    case 'B': {   // Set Brightness
        commandSetBrightness();
        break;
    }
          
    case 'P': {   // Set Pixel
        commandSetPixel();
        break;
    }

    case 'I': {   // Receive new image
        commandImage();
        break;
    }
    #endif
    #endif

    case '!': {   // Receive control
        commandController();
        break;
     }
  }
}

void error_halt(const __FlashStringHelper * err)
{
  while (1)
  {
    Serial.println(err);
    for (int i = 0; i < 5; i++)
    {
      BookWorm.setLedLeftOn();
      BookWorm.setLedRightOff();
      delay(150);
      BookWorm.setLedLeftOff();
      BookWorm.setLedRightOn();
      delay(150);
    }
  }
}
