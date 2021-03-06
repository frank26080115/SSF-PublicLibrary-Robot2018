#include <Arduino.h>
#include "BookWorm.h"
#include <IRremote_Include.h>

static bool irRxInited = false;
static IRrecv irrecv(pinTvRemoteInput);
static IRsend irsend;

static int decodeTvCommand(decode_results *results);

void cBookWorm::initTvRemote(void)
{
	pinMode(pinTvRemoteInput, INPUT);
	digitalWrite(pinTvRemoteInput, HIGH);
}

int cBookWorm::readTvRemote(void)
{
	decode_results  results;

	if (irRxInited == false) {
		irRxInited = true;
		initTvRemote();
		irrecv.enableIRIn();
		return TVREMOTE_KEY_NONE;
	}

	if (irrecv.decode(&results))
	{
		int res = decodeTvCommand(&results);
		this->printf(F("TV-RX 0x%02X 0x%04X"), results.decode_type, results.value);
		this->printf(F(" %d\r\n"), results.bits);
		//if (results.bits <= 0)
		//{
		//	return TVREMOTE_KEY_REPEATLAST;
		//}
		irrecv.resume();
		return res;
	}
	else
	{
		return TVREMOTE_KEY_NONE;
	}

	return TVREMOTE_KEY_NONE;
}

void cBookWorm::sendTvRemote(uint8_t brand, unsigned long data, unsigned int nbits)
{
	switch (brand) {
		#if SEND_NEC
		case TVREMOTE_BRAND_NEC: irsend.sendNEC(data, nbits); return ;
		#endif
		#if SEND_SONY
		case TVREMOTE_BRAND_SONY: irsend.sendSony(data, nbits); return ;
		#endif
		#if SEND_RC5
		case TVREMOTE_BRAND_RC5: irsend.sendRC5(data, nbits); return ;
		#endif
		#if SEND_RC6
		case TVREMOTE_BRAND_RC6: irsend.sendRC6(data, nbits); return ;
		#endif
		#if SEND_DISH
		case TVREMOTE_BRAND_DISH: irsend.sendDISH(data, nbits); return ;
		#endif
		#if SEND_SHARP
		case TVREMOTE_BRAND_SHARP: irsend.sendSharp(data, nbits); return ;
		#endif
		#if SEND_JVC
		case TVREMOTE_BRAND_JVC: irsend.sendJVC(data, nbits, false); return ;
		#endif
		//#if SEND_SANYO
		//case TVREMOTE_BRAND_ANYO: irsend.sendSanyo(data, nbits); return ;
		//#endif
		//#if SEND_MITSUBISHI
		//case TVREMOTE_BRAND_MITSUBISHI: irsend.sendMitsubishi(data, nbits); return ;
		//#endif
		#if SEND_SAMSUNG
		case TVREMOTE_BRAND_SAMSUNG: irsend.sendSAMSUNG(data, nbits); return ;
		#endif
		#if SEND_LG
		case TVREMOTE_BRAND_LG: irsend.sendLG(data, nbits); return ;
		#endif
		#if SEND_WHYNTER
		case TVREMOTE_BRAND_WHYNTER: irsend.sendWhynter(data, nbits); return ;
		#endif
		#if SEND_AIWA_RC_T501
		case TVREMOTE_BRAND_AIWA_RC_T501: irsend.sendAiwaRCT501(data); return ;
		#endif
		#if SEND_PANASONIC
		case TVREMOTE_BRAND_PANASONIC: irsend.sendPanasonic(nbits, data); return ;
		#endif
		#if SEND_DENON
		case TVREMOTE_BRAND_DENON: irsend.sendDenon(data, nbits); return ;
		#endif
		#if SEND_HEXBUG
		case TVREMOTE_BRAND_HEXBUG: irsend.sendHexbug(data, nbits); return ;
		#endif
		default: return;
	}
}

static int decodeTvCommand(decode_results *results)
{
	uint8_t d = results->decode_type;
	uint16_t x = results->value;
	if (d == TVREMOTE_BRAND_SONY)
	{
		switch (x)
		{
			case 0x0A70:
				return TVREMOTE_KEY_SELECT;
			case 0x02D0:
				return TVREMOTE_KEY_LEFT;
			case 0x0CD0:
				return TVREMOTE_KEY_RIGHT;
			case 0x02F0:
				return TVREMOTE_KEY_UP;
			case 0x0AF0:
				return TVREMOTE_KEY_DOWN;
			case 0x72E9:
				return TVREMOTE_KEY_YELLOW;
			case 0x12E9:
				return TVREMOTE_KEY_BLUE;
			case 0x52E9:
				return TVREMOTE_KEY_RED;
			case 0x32E9:
				return TVREMOTE_KEY_GREEN;
			case 0x2CE9:
				return TVREMOTE_KEY_PLAY;
			case 0x0CE9:
				return TVREMOTE_KEY_STOP;
			case 0x0010: return TVREMOTE_KEY_1;
			case 0x0810: return TVREMOTE_KEY_2;
			case 0x0410: return TVREMOTE_KEY_3;
			case 0x0C10: return TVREMOTE_KEY_4;
			case 0x0210: return TVREMOTE_KEY_5;
			case 0x0A10: return TVREMOTE_KEY_6;
			case 0x0610: return TVREMOTE_KEY_7;
			case 0x0E10: return TVREMOTE_KEY_8;
			case 0x0110: return TVREMOTE_KEY_9;
			case 0x0910: return TVREMOTE_KEY_0;
			default:
				return TVREMOTE_KEY_UNKNOWN;
		}
	}
	if (d == TVREMOTE_BRAND_SAMSUNG)
	{
		switch (x)
		{
			case 0x16E9:
				return TVREMOTE_KEY_SELECT;
			case 0xA659:
				return TVREMOTE_KEY_LEFT;
			case 0x46B9:
				return TVREMOTE_KEY_RIGHT;
			case 0x06F9:
				return TVREMOTE_KEY_UP;
			case 0x8679:
				return TVREMOTE_KEY_DOWN;
			case 0xA857:
				return TVREMOTE_KEY_YELLOW;
			case 0x6897:
				return TVREMOTE_KEY_BLUE;
			case 0x36C9:
				return TVREMOTE_KEY_RED;
			case 0x28D7:
				return TVREMOTE_KEY_GREEN;
			case 0xE21D:
				return TVREMOTE_KEY_PLAY;
			case 0x629D:
				return TVREMOTE_KEY_STOP;
			case 0x20DF: return TVREMOTE_KEY_1;
			case 0xA05F: return TVREMOTE_KEY_2;
			case 0x609F: return TVREMOTE_KEY_3;
			case 0x10EF: return TVREMOTE_KEY_4;
			case 0x906F: return TVREMOTE_KEY_5;
			case 0x50AF: return TVREMOTE_KEY_6;
			case 0x30CF: return TVREMOTE_KEY_7;
			case 0xB04F: return TVREMOTE_KEY_8;
			case 0x708F: return TVREMOTE_KEY_9;
			case 0x8877: return TVREMOTE_KEY_0;
			default:
				return TVREMOTE_KEY_UNKNOWN;
		}
	}
	if (d == TVREMOTE_BRAND_HEXBUG)
	{
		static uint8_t player = 0;
		uint8_t p, xx;
		if (results->bits != 7 && results->bits != 8)
		{
			return TVREMOTE_KEY_UNKNOWN;
		}
		if (results->bits == 8)
		{
			xx = x & 0xF8;
			p = x & 0x07;
			if (xx != 0 && player == 0)
			{
				player = p;
			}
			if (player != p)
			{
				return TVREMOTE_KEY_UNKNOWN;
			}
			switch (xx)
			{
				case 0x80:
					return TVREMOTE_KEY_UP;
				case 0x40:
					return TVREMOTE_KEY_LEFT;
				case 0x20:
					return TVREMOTE_KEY_RIGHT;
				case 0x10:
					return TVREMOTE_KEY_DOWN;
				case 0x08:
					return TVREMOTE_KEY_SELECT;
				default:
					return TVREMOTE_KEY_UNKNOWN;
			}
		}
		else if (results->bits == 7)
		{
			return TVREMOTE_KEY_UNKNOWN;
		}
	}
	return TVREMOTE_KEY_UNKNOWN;
}
