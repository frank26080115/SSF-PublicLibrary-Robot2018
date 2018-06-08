// a classic trick from http://www.instructables.com/id/Music-Playing-Alarm-Clock/
// original inspiration: https://forum.arduino.cc/index.php/topic,7245.0.html
// edited to be much more memory efficient


// addresses for custom chars
#define upperlineaddr 0
#define ulc upperlineaddr
#define lowerlineaddr 1
#define llc lowerlineaddr
#define bothlineaddr 2
#define blc bothlineaddr
#define largecolonaddr 3
#define largeperiodaddr 4
#define xxc 0xFF
#define ssc 0x20

// below are custom LCD characters, stored in flash

const uint8_t upperlinechar[8] PROGMEM = {
  0b00011111,
  0b00011111,
  0b00011111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t lowerlinechar[8] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011111,
  0b00011111,
  0b00011111
};

const uint8_t bothlinechar[8] PROGMEM = {
  0b00011111,
  0b00011111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011111,
  0b00011111
};

/*
const uint8_t largecolonleftchar[8] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000001,
  0b00000011,
  0b00000011,
  0b00000001,
  0b00000000,
  0b00000000
};

const uint8_t largecolonrightchar[8] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00010000,
  0b00011000,
  0b00011000,
  0b00010000,
  0b00000000,
  0b00000000
};

const uint8_t upchar[8] PROGMEM = {
  0b00000000,
  0b00000100,
  0b00001110,
  0b00010101,
  0b00000100,
  0b00000100,
  0b00000100,
  0b00000000
};

const uint8_t downchar[8] PROGMEM = {
  0b00000000,
  0b00000100,
  0b00000100,
  0b00000100,
  0b00010101,
  0b00001110,
  0b00000100,
  0b00000000
};
*/

const uint8_t largecolon[8] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000100,
  0b00001110,
  0b00001110,
  0b00000100,
  0b00000000,
  0b00000000
};

const uint8_t largeperiod[8] PROGMEM = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000100,
  0b00001110,
  0b00000100
};

const uint8_t numchar[10][6] PROGMEM = {
  {
    xxc, ulc, xxc,
    xxc, llc, xxc,
  },
  {
    ulc, xxc, ssc,
    llc, xxc, llc,
  },
  {
    ulc, blc, xxc,
    xxc, llc, llc,
  },
  {
    ulc, blc, xxc,
    llc, llc, xxc,
  },
  {
    xxc, llc, xxc,
    ssc, ssc, xxc,
  },
  {
    xxc, blc, ulc,
    llc, llc, xxc,
  },
  {
    xxc, blc, ulc,
    xxc, llc, xxc,
  },
  {
    ulc, ulc, xxc,
    ssc, ssc, xxc,
  },
  {
    xxc, blc, xxc,
    xxc, llc, xxc,
  },
  {
    xxc, blc, xxc,
    llc, llc, xxc,
  }
};

void lcd_bigNum(uint8_t num, uint8_t col)
{
	lcd.setCursor(col,0);
	lcd.write(pgm_read_byte(&numchar[num][0]));
	lcd.write(pgm_read_byte(&numchar[num][1]));
	lcd.write(pgm_read_byte(&numchar[num][2]));
	lcd.setCursor(col,1);
	lcd.write(pgm_read_byte(&numchar[num][3]));
	lcd.write(pgm_read_byte(&numchar[num][4]));
	lcd.write(pgm_read_byte(&numchar[num][5]));
}

void lcd_bigNumInit()
{
	// set all custom fonts
	uint8_t temp[8];
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		temp[i] = pgm_read_byte(&upperlinechar[i]);
	}
	lcd.createChar(upperlineaddr, temp);

	for (i = 0; i < 8; i++)
	{
		temp[i] = pgm_read_byte(&lowerlinechar[i]);
	}
	lcd.createChar(lowerlineaddr, temp);

	for (i = 0; i < 8; i++)
	{
		temp[i] = pgm_read_byte(&bothlinechar[i]);
	}
	lcd.createChar(bothlineaddr, temp);

	for (i = 0; i < 8; i++)
	{
		temp[i] = pgm_read_byte(&largecolon[i]);
	}
	lcd.createChar(largecolonaddr, temp);

	for (i = 0; i < 8; i++)
	{
		temp[i] = pgm_read_byte(&largeperiod[i]);
	}
	lcd.createChar(largeperiodaddr, temp);
}

void lcd_printTime(uint32_t msec, uint8_t col, bool dots)
{
	uint32_t msec100 = (msec / 100) % 10;
	uint32_t secsTotal = msec / 1000;
	uint32_t secsNoMin = secsTotal % 60;
	uint32_t mins = (secsTotal / 60) % 10;
	uint8_t secsMod10 = secsNoMin % 10;
	uint8_t deciSecs = secsNoMin / 10;

	uint8_t x = 0;

	// blank out start
	for (x = 0; x < col && col <= 16 && col > 0; x++)
	{
		lcd.setCursor(x, 0);
		lcd.write(' ');
		lcd.setCursor(x, 1);
		lcd.write(' ');
	}

	x = col;

	lcd_bigNum(mins, x);
	x += 3;
	lcd.setCursor(x, 0);
	lcd.write(dots ? largecolonaddr : ' ');
	lcd.setCursor(x, 1);
	lcd.write(dots ? largecolonaddr : ' ');
	x += 1;
	lcd_bigNum(deciSecs, x);
	x += 3;
	lcd.setCursor(x, 0);
	lcd.write(' ');
	lcd.setCursor(x, 1);
	lcd.write(' ');
	x += 1;
	lcd_bigNum(secsMod10, x);
	x += 3;
	lcd.setCursor(x, 1);
	lcd.write(dots ? largeperiodaddr : ' ');
	x += 1;
	lcd_bigNum(msec100, x);
	x += 3;

	// blank out end
	for (; x < 16 && col <= 16 && col > 0; x++)
	{
		lcd.setCursor(x, 0);
		lcd.write(' ');
		lcd.setCursor(x, 1);
		lcd.write(' ');
	}
}
