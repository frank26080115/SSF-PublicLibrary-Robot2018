#include <Arduino.h>
#include "BookWorm.h"
#include <avr/pgmspace.h>
#include "WString.h"

static int printf_putchar(char c, FILE *fp)
{
	Serial.write((uint8_t)c);
	return 0;
}

int cBookWorm::printf(const char *format, ...)
{
	FILE f;
	va_list ap;

	fdev_setup_stream(&f, printf_putchar, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&f, this);
	va_start(ap, format);
	return vfprintf(&f, format, ap);
}

int cBookWorm::printf(const __FlashStringHelper *format, ...)
{
	FILE f;
	va_list ap;

	fdev_setup_stream(&f, printf_putchar, NULL, _FDEV_SETUP_WRITE);
	fdev_set_udata(&f, this);
	va_start(ap, format);
	return vfprintf_P(&f, (const char *)format, ap);
}