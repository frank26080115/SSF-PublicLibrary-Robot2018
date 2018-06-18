/*
 * the code here is taken from https://github.com/adafruit/Bluefruit_LE_Connect_v2/blob/master/Bluefruit/Platform/Arduino/Neopixel_bluefruitmicro/Neopixel/Neopixel.ino
 * for an absolute beginner, it will be hard to understand what this code does
 * it requires you to understand pointer arithmatic, how neopixels work
 * and what the Android/iOS app is actually sending
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NEOPIXEL_VERSION_STRING "Neopixel v2.0"

#define MAXCOMPONENTS  4
uint8_t *pixelBuffer = NULL;
uint8_t width = 0;
uint8_t height = 0;
uint8_t stride;
uint8_t componentsValue;
bool is400Hz;
uint8_t components = 3;     // only 3 and 4 are valid values

void swapBuffers()
{
  uint8_t *base_addr = pixelBuffer;
  int pixelIndex = 0;
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i < width; i++) {
      if (components == 3) {
        neopixel.setPixelColor(pixelIndex, neopixel.Color(*base_addr, *(base_addr+1), *(base_addr+2) ));
      }
      else {
        neopixel.setPixelColor(pixelIndex, neopixel.Color(*base_addr, *(base_addr+1), *(base_addr+2), *(base_addr+3) ));
      }
      base_addr+=components;
      pixelIndex++;
    }
    pixelIndex += stride - width;   // move pixelIndex to the next row (take into account the stride)
  }
  neopixel.show();

}

void commandVersion() {
  Serial.println(F("Command: Version check"));
  sendResponse(NEOPIXEL_VERSION_STRING);
}

void commandSetup() {
  Serial.println(F("Command: Setup"));

  width = ble.read();
  height = ble.read();
  stride = ble.read();
  componentsValue = ble.read();
  is400Hz = ble.read();
  
  neoPixelType pixelType;
  pixelType = componentsValue + (is400Hz ? NEO_KHZ400 : NEO_KHZ800);

  components = (componentsValue == NEO_RGB || componentsValue == NEO_RBG || componentsValue == NEO_GRB || componentsValue == NEO_GBR || componentsValue == NEO_BRG || componentsValue == NEO_BGR) ? 3:4;
  
  BookWorm.printf(F("\tsize: %dx%d"), width, height);
  BookWorm.printf(F("\tstride: %d"), stride);
  BookWorm.printf(F("\tpixelType %d"), pixelType );
  BookWorm.printf(F("\tcomponents: %d"), components);

  if (pixelBuffer != NULL) {
      delete[] pixelBuffer;
  }

  uint32_t size = width*height;
  pixelBuffer = new uint8_t[size*components];
  neopixel.updateLength(size);
  neopixel.updateType(pixelType);
  neopixel.setPin(NEOPIXEL_PIN);

  // Done
  sendResponse("OK");
}

void commandSetBrightness() {
  Serial.println(F("Command: SetBrightness"));

   // Read value
  uint8_t brightness = ble.read();

  // Set brightness
  neopixel.setBrightness(brightness);

  // Refresh pixels
  swapBuffers();

  // Done
  sendResponse("OK");
}

void commandClearColor() {
  Serial.println(F("Command: ClearColor"));

  // Read color
  uint8_t color[MAXCOMPONENTS];
  for (int j = 0; j < components;) {
    if (ble.available()) {
      color[j] = ble.read();
      j++;
    }
  }

  // Set all leds to color
  int size = width * height;
  uint8_t *base_addr = pixelBuffer;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < components; j++) {
      *base_addr = color[j];
      base_addr++;
    }
  }

  // Swap buffers
  Serial.println(F("ClearColor completed"));
  swapBuffers();

  if (components == 3) {
    BookWorm.printf(F("\tclear (%d, %d, %d)"), color[0], color[1], color[2] );
  }
  else {    
    BookWorm.printf(F("\tclear (%d, %d, %d, %d)"), color[0], color[1], color[2], color[3] );
  }
  
  // Done
  sendResponse("OK");
}

void commandSetPixel() {
  Serial.println(F("Command: SetPixel"));

  // Read position
  uint8_t x = ble.read();
  uint8_t y = ble.read();

  // Read colors
  uint32_t pixelOffset = y*width+x;
  uint32_t pixelDataOffset = pixelOffset*components;
  uint8_t *base_addr = pixelBuffer+pixelDataOffset;
  for (int j = 0; j < components;) {
    if (ble.available()) {
      *base_addr = ble.read();
      base_addr++;
      j++;
    }
  }

  // Set colors
  uint32_t neopixelIndex = y*stride+x;
  uint8_t *pixelBufferPointer = pixelBuffer + pixelDataOffset;
  uint32_t color;
  if (components == 3) {
    color = neopixel.Color( *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2) );
    BookWorm.printf(F("\tcolor (%d, %d, %d)"), *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2) );
  }
  else {
    color = neopixel.Color( *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2), *(pixelBufferPointer+3) );
    BookWorm.printf(F("\tcolor (%d, %d, %d, %d)"), *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2), *(pixelBufferPointer+3) );    
  }
  neopixel.setPixelColor(neopixelIndex, color);
  neopixel.show();

  // Done
  sendResponse("OK");
}

void commandImage() {
  BookWorm.printf("Command: Image %dx%d, %d, %d", width, height, components, stride);
  
  // Receive new pixel buffer
  int size = width * height;
  uint8_t *base_addr = pixelBuffer;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < components;) {
      if (ble.available()) {
        *base_addr = ble.read();
        base_addr++;
        j++;
      }
    }
  }

  // Swap buffers
  Serial.println(F("Image received"));
  swapBuffers();

  // Done
  sendResponse("OK");
}

void sendResponse(char *response) {
    BookWorm.printf(F("Send Response: %s"), response);
    ble.write(response, strlen(response)*sizeof(char));
}

