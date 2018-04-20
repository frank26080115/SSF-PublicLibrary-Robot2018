## What is "dongle"?

The problem is that the library's laptops are locked-down. I can't install device drivers or applications on them, and I can't depend on the library's IT staff to do so either. The Arduino Nanos that cost only $4 each have a CH430 chip as the USB-to-UART converter and that definitely needs a driver to be installed in order to work.

Some form of USB-to-UART is needed in order to communicate with the Arduino Nano bootloader. Ideally, with auto-reset capability.

Well... the library's laptops already have Arduino 1.8.1 installed. Which means that the default virtual COM drivers for Arduino's ATmega8U2 must already be installed as well.

The "dongle" directory contains code for a PSoC that turns it into a USB-to-UART converter. It will use the same VID and PID as the Arduino's ATmega8U2.

The hardware will be the detachable KitProg from Cypress PSoC dev kits. I have plenty of these to give away because we typically break them off from the dev kits at work.

There is no capacitor for the DTS signal that is meant for auto-reset, instead, a simple pulse with a delay simulates the capacitor.

So in the end, for zero cost to me personally, I provide about a dozen USB-to-serial converters to the library that works without additional driver installation. :-)