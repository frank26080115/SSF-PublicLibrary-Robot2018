#include <Arduino.h>
#include "BookWorm.h"
#include <IRremote_Include.h>

static bool irRxInited = false;
static IRrecv irrecv(pinTvRemoteInput);
static IRsend irsend;

static int decodeTvCommand(decode_results *results);

int cBookWorm::readTvRemote(void)
{
	decode_results  results;

	if (irRxInited == false) {
		irRxInited = true;
		irrecv.enableIRIn();
		return TVREMOTE_KEY_NONE;
	}

	if (irrecv.decode(&results))
	{
		this->printf(F("TV-RX 0x%02X 0x%04X %d\r\n"), results.decode_type, results.value, results.bits);
		return decodeTvCommand(&results);
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
		default: return;
	}
}

static int decodeTvCommand(decode_results *results)
{
	uint8_t d = results->decode_type;
	uint16_t x = results->value;
	#if DECODE_NEC
	if (d == NEC)
	{
		if (0
			|| x == 0x000B
			|| x == 0x004C
			|| x == 0x004D
			|| x == 0x005E
			|| x == 0x00BA
			|| x == 0x0031
			|| x == 0x0033
			|| x == 0x00C7
			|| x == 0x0091
			|| x == 0x0060
			|| x == 0x002F
			|| x == 0x00CE
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x000C
			|| x == 0x0010
			|| x == 0x0003
			|| x == 0x001B
			|| x == 0x000E
			|| x == 0x0083
			|| x == 0x0027
			|| x == 0x0023
			|| x == 0x0070
			|| x == 0x00D0
			|| x == 0x009C
			|| x == 0x007E
			|| x == 0x00D7
			|| x == 0x00CB
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x000F
			|| x == 0x000D
			|| x == 0x0058
			|| x == 0x0035
			|| x == 0x0030
			|| x == 0x009B
			|| x == 0x00D2
			|| x == 0x00D3
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0054
			|| x == 0x008A
			|| x == 0x004F
			|| x == 0x0040
			|| x == 0x00C1
			|| x == 0x002C
			|| x == 0x0039
			|| x == 0x00D1
			|| x == 0x009D
			|| x == 0x003D
			|| x == 0x00C3
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0013
			|| x == 0x004A
			|| x == 0x0017
			|| x == 0x008E
			|| x == 0x00CA
			|| x == 0x0053
			|| x == 0x005D
			|| x == 0x0032
			|| x == 0x0093
			|| x == 0x0067
			|| x == 0x00D5
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0044
			|| x == 0x0052
			|| x == 0x0087
			|| x == 0x0004
			|| x == 0x0016
			|| x == 0x005F
			|| x == 0x005B
			|| x == 0x0086
			|| x == 0x00C0
			|| x == 0x0099
			|| x == 0x0038
			|| x == 0x0066
			|| x == 0x008D
			|| x == 0x00FA
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0045
			|| x == 0x0051
			|| x == 0x0015
			|| x == 0x0001
			|| x == 0x008C
			|| x == 0x0085
			|| x == 0x0036
			|| x == 0x0095
			|| x == 0x0065
			|| x == 0x003F
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0046
			|| x == 0x004B
			|| x == 0x0014
			|| x == 0x0050
			|| x == 0x0012
			|| x == 0x008B
			|| x == 0x00C9
			|| x == 0x0084
			|| x == 0x003E
			|| x == 0x0028
			|| x == 0x00B0
			|| x == 0x0064
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0047
			|| x == 0x0048
			|| x == 0x0090
			|| x == 0x00EE
			|| x == 0x00F1
			|| x == 0x0022
			|| x == 0x0098
			|| x == 0x0069
			|| x == 0x00CD
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0049
			|| x == 0x0007
			|| x == 0x002A
			|| x == 0x0068
			|| x == 0x008F
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0011
			|| x == 0x0002
			|| x == 0x00CF
			|| x == 0x000A
			|| x == 0x0081
			|| x == 0x0088
			|| x == 0x0042
			|| x == 0x002D
			|| x == 0x003B
			|| x == 0x0097
			|| x == 0x0061
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0019
			|| x == 0x001E
			|| x == 0x001C
			|| x == 0x001D
			|| x == 0x0043
			|| x == 0x00C4
			|| x == 0x00C8
			|| x == 0x0006
			|| x == 0x001F
			|| x == 0x0057
			|| x == 0x005A
			|| x == 0x005C
			|| x == 0x0055
			|| x == 0x0080
			|| x == 0x0021
			|| x == 0x0025
			|| x == 0x003A
			|| x == 0x0077
			|| x == 0x0076
			|| x == 0x0092
			|| x == 0x0094
			|| x == 0x0096
			|| x == 0x00AD
			|| x == 0x00B8
			|| x == 0x007D
			|| x == 0x0072
			|| x == 0x0073
			|| x == 0x006A
			|| x == 0x00C2
			|| x == 0x00DA
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x004E
			|| x == 0x0005
			|| x == 0x0041
			|| x == 0x0089
			|| x == 0x00C5
			|| x == 0x00C6
			|| x == 0x0029
			|| x == 0x002B
			|| x == 0x002E
			|| x == 0x009E
			|| x == 0x0062
			|| x == 0x0063
			|| x == 0x003C
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0056
			|| x == 0x0009
			|| x == 0x0059
			|| x == 0x0037
			|| x == 0x009F
			|| x == 0x00D4
			|| x == 0x00A0
			|| x == 0x00A1
			|| x == 0x00A2
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0018
			|| x == 0x001A
			|| x == 0x0082
			|| x == 0x0024
			|| x == 0x0020
			|| x == 0x00F0
			|| x == 0x009A
			|| x == 0x0026
			|| x == 0x00D6
			|| x == 0x006F
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
	}
	#endif // DECODE_NEC
	#if DECODE_PANASONIC
	if (d == PANASONIC)
	{
		if (0
			|| x == 0x0020
			|| x == 0x00D2
			|| x == 0x00B0
			|| x == 0x0024
			|| x == 0x0063
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0021
			|| x == 0x00D3
			|| x == 0x00B1
			|| x == 0x0025
			|| x == 0x0064
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x0010
			|| x == 0x00A0
			|| x == 0x0090
			|| x == 0x0001
			|| x == 0x0080
			|| x == 0x0084
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0011
			|| x == 0x00A1
			|| x == 0x0091
			|| x == 0x009F
			|| x == 0x003B
			|| x == 0x0002
			|| x == 0x0081
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0012
			|| x == 0x00A2
			|| x == 0x0092
			|| x == 0x0003
			|| x == 0x0082
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0013
			|| x == 0x00A3
			|| x == 0x0093
			|| x == 0x0004
			|| x == 0x0083
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0014
			|| x == 0x0026
			|| x == 0x0094
			|| x == 0x0005
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0015
			|| x == 0x0027
			|| x == 0x0095
			|| x == 0x0006
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0016
			|| x == 0x00A6
			|| x == 0x0096
			|| x == 0x0007
			|| x == 0x0086
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0017
			|| x == 0x00A7
			|| x == 0x0097
			|| x == 0x0008
			|| x == 0x0087
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0018
			|| x == 0x00A8
			|| x == 0x002A
			|| x == 0x0098
			|| x == 0x0009
			|| x == 0x0088
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0019
			|| x == 0x0085
			|| x == 0x00A9
			|| x == 0x002B
			|| x == 0x0099
			|| x == 0x0089
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x003D
			|| x == 0x003F
			|| x == 0x0032
			|| x == 0x00BD
			|| x == 0x00DD
			|| x == 0x00BF
			|| x == 0x008D
			|| x == 0x00D6
			|| x == 0x0079
			|| x == 0x000B
			|| x == 0x000D
			|| x == 0x000C
			|| x == 0x00AD
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x00A4
			|| x == 0x00CA
			|| x == 0x0034
			|| x == 0x0022
			|| x == 0x0036
			|| x == 0x004A
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x00A5
			|| x == 0x00CB
			|| x == 0x0035
			|| x == 0x0065
			|| x == 0x0023
			|| x == 0x0037
			|| x == 0x0049
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
	}
	#endif // DECODE_PANASONIC
	#if DECODE_JVC
	if (d == JVC)
	{
		if (0
			|| x == 0x001C
			|| x == 0x006C
			|| x == 0x0040
			|| x == 0x000B
			|| x == 0x004C
			|| x == 0x0010
			|| x == 0x001D
			|| x == 0x0002
			|| x == 0x00D7
			|| x == 0x00DD
			|| x == 0x005A
			|| x == 0x0097
			|| x == 0x008E
			|| x == 0x0086
			|| x == 0x00C0
			|| x == 0x0093
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0061
			|| x == 0x0021
			|| x == 0x0011
			|| x == 0x006F
			|| x == 0x0070
			|| x == 0x0079
			|| x == 0x007F
			|| x == 0x0004
			|| x == 0x002E
			|| x == 0x00A1
			|| x == 0x0050
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0062
			|| x == 0x0022
			|| x == 0x0005
			|| x == 0x0012
			|| x == 0x0072
			|| x == 0x007A
			|| x == 0x009D
			|| x == 0x00A2
			|| x == 0x0051
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0063
			|| x == 0x0023
			|| x == 0x0013
			|| x == 0x007B
			|| x == 0x00BD
			|| x == 0x00A3
			|| x == 0x0052
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0064
			|| x == 0x0024
			|| x == 0x0014
			|| x == 0x0074
			|| x == 0x007C
			|| x == 0x0008
			|| x == 0x00A4
			|| x == 0x0053
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0065
			|| x == 0x0025
			|| x == 0x0015
			|| x == 0x0075
			|| x == 0x007D
			|| x == 0x0009
			|| x == 0x00A5
			|| x == 0x0054
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0066
			|| x == 0x0026
			|| x == 0x0016
			|| x == 0x0076
			|| x == 0x007E
			|| x == 0x000A
			|| x == 0x0036
			|| x == 0x00A6
			|| x == 0x0055
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0067
			|| x == 0x0027
			|| x == 0x0017
			|| x == 0x0078
			|| x == 0x000C
			|| x == 0x00A7
			|| x == 0x0056
			|| x == 0x0035
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0068
			|| x == 0x0018
			|| x == 0x0028
			|| x == 0x000D
			|| x == 0x0057
			|| x == 0x00C8
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0069
			|| x == 0x0019
			|| x == 0x0029
			|| x == 0x000E
			|| x == 0x0058
			|| x == 0x00C9
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x006E
			|| x == 0x0033
			|| x == 0x0060
			|| x == 0x0020
			|| x == 0x002A
			|| x == 0x0077
			|| x == 0x002F
			|| x == 0x004F
			|| x == 0x00CA
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x001E
			|| x == 0x00BE
			|| x == 0x0071
			|| x == 0x0096
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x001F
			|| x == 0x00BF
			|| x == 0x0073
			|| x == 0x000F
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0001
			|| x == 0x0006
			|| x == 0x004E
			|| x == 0x005E
			|| x == 0x0084
			|| x == 0x001A
			|| x == 0x009E
			|| x == 0x0098
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x005F
			|| x == 0x0085
			|| x == 0x0030
			|| x == 0x0007
			|| x == 0x009F
			|| x == 0x0099
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
	}
	#endif // DECODE_JVC
	#if DECODE_SHARP
	if (d == SHARP)
	{
		if (0
			|| x == 0x0014
			|| x == 0x00A4
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0015
			|| x == 0x00A5
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x0017
			|| x == 0x0016
			|| x == 0x0096
			|| x == 0x0095
			|| x == 0x001B
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0001
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0002
			|| x == 0x003C
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0003
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0004
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0005
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0006
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0007
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0008
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0009
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x000A
			|| x == 0x002F
			|| x == 0x0030
			|| x == 0x0031
			|| x == 0x000B
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x0011
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x0012
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
	}
	#endif // DECODE_SHARP
	#if DECODE_DENON
	if (d == DENON)
	{
		if (0
			|| x == 0x00DD
			|| x == 0x0075
			|| x == 0x0073
			|| x == 0x00F0
			|| x == 0x00E2
			|| x == 0x0003
			|| x == 0x0029
			|| x == 0x0050
			|| x == 0x005D
			|| x == 0x00F8
			|| x == 0x00A2
			|| x == 0x007A
			|| x == 0x00DF
			|| x == 0x00C0
			|| x == 0x00B1
			|| x == 0x00E0
			|| x == 0x0172
			|| x == 0x0017
			|| x == 0x0112
			|| x == 0x000C
			|| x == 0x0028
			|| x == 0x00AB
			|| x == 0x00AC
			|| x == 0x0055
			|| x == 0x0056
			|| x == 0x00D0
			|| x == 0x00E1
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x004C
			|| x == 0x00C1
			|| x == 0x0006
			|| x == 0x0042
			|| x == 0x0001
			|| x == 0x00A1
			|| x == 0x0064
			|| x == 0x0082
			|| x == 0x00B0
			|| x == 0x00E8
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x004D
			|| x == 0x008D
			|| x == 0x004E
			|| x == 0x001C
			|| x == 0x0066
			|| x == 0x0058
			|| x == 0x00D9
			|| x == 0x0110
			|| x == 0x00D8
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0081
			|| x == 0x0030
			|| x == 0x0041
			|| x == 0x00A0
			|| x == 0x004B
			|| x == 0x00CA
			|| x == 0x009A
			|| x == 0x00CF
			|| x == 0x00B9
			|| x == 0x0013
			|| x == 0x0019
			|| x == 0x002B
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x008E
			|| x == 0x004F
			|| x == 0x0065
			|| x == 0x008C
			|| x == 0x009F
			|| x == 0x00DA
			|| x == 0x00D2
			|| x == 0x0014
			|| x == 0x0111
			|| x == 0x0057
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x00F1
			|| x == 0x0096
			|| x == 0x00D6
			|| x == 0x0092
			|| x == 0x0052
			|| x == 0x001E
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x00F2
			|| x == 0x0059
			|| x == 0x0097
			|| x == 0x00D5
			|| x == 0x0093
			|| x == 0x0016
			|| x == 0x0053
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0002
			|| x == 0x0007
			|| x == 0x0043
			|| x == 0x00C2
			|| x == 0x0083
			|| x == 0x0011
			|| x == 0x001F
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0004
			|| x == 0x000A
			|| x == 0x0045
			|| x == 0x00A4
			|| x == 0x0067
			|| x == 0x00C4
			|| x == 0x0085
			|| x == 0x00E3
			|| x == 0x00B3
			|| x == 0x0022
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0005
			|| x == 0x0046
			|| x == 0x00A5
			|| x == 0x0068
			|| x == 0x00C5
			|| x == 0x0086
			|| x == 0x00B4
			|| x == 0x000B
			|| x == 0x0023
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0044
			|| x == 0x00A3
			|| x == 0x00C3
			|| x == 0x0084
			|| x == 0x00B2
			|| x == 0x0012
			|| x == 0x0020
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0009
			|| x == 0x004A
			|| x == 0x00A9
			|| x == 0x00C9
			|| x == 0x008A
			|| x == 0x00CD
			|| x == 0x00DB
			|| x == 0x02C7
			|| x == 0x00B8
			|| x == 0x0010
			|| x == 0x0018
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0047
			|| x == 0x00A6
			|| x == 0x0069
			|| x == 0x00C6
			|| x == 0x0087
			|| x == 0x00CC
			|| x == 0x00B5
			|| x == 0x0015
			|| x == 0x0024
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0048
			|| x == 0x00C7
			|| x == 0x0088
			|| x == 0x02DD
			|| x == 0x00B6
			|| x == 0x000E
			|| x == 0x0026
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0049
			|| x == 0x00A8
			|| x == 0x00C8
			|| x == 0x0089
			|| x == 0x00CE
			|| x == 0x00D3
			|| x == 0x02DE
			|| x == 0x00B7
			|| x == 0x000F
			|| x == 0x0027
		) {
			return TVREMOTE_KEY_8;
		}
	}
	#endif // DECODE_DENON
	#if DECODE_MITSUBISHI
	if (d == MITSUBISHI)
	{
		if (0
			|| x == 0x0001
			|| x == 0x0007
			|| x == 0x000D
			|| x == 0x00C1
			|| x == 0x0081
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0019
			|| x == 0x00C8
			|| x == 0x0047
			|| x == 0x0088
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0009
			|| x == 0x00C9
			|| x == 0x0089
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x000A
			|| x == 0x001A
			|| x == 0x0076
			|| x == 0x008A
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x0010
			|| x == 0x00D0
			|| x == 0x0050
			|| x == 0x0090
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0011
			|| x == 0x00C0
			|| x == 0x0046
			|| x == 0x0080
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0012
			|| x == 0x000F
			|| x == 0x007E
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0018
			|| x == 0x00D8
			|| x == 0x0054
			|| x == 0x0014
			|| x == 0x0098
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0020
			|| x == 0x0003
			|| x == 0x00E0
			|| x == 0x0055
			|| x == 0x00A0
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0022
			|| x == 0x002D
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0028
			|| x == 0x0004
			|| x == 0x00E8
			|| x == 0x0056
			|| x == 0x0016
			|| x == 0x00A8
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x002A
			|| x == 0x000E
			|| x == 0x001E
			|| x == 0x0035
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x0030
			|| x == 0x0005
			|| x == 0x000B
			|| x == 0x00F0
			|| x == 0x0053
			|| x == 0x0017
			|| x == 0x00B0
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0032
			|| x == 0x0002
			|| x == 0x0015
			|| x == 0x001C
			|| x == 0x0044
			|| x == 0x0013
			|| x == 0x00B2
			|| x == 0x0082
			|| x == 0x004A
			|| x == 0x0042
			|| x == 0x0092
			|| x == 0x00CA
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0038
			|| x == 0x0006
			|| x == 0x000C
			|| x == 0x00F8
			|| x == 0x0052
			|| x == 0x00B8
		) {
			return TVREMOTE_KEY_8;
		}
	}
	#endif // DECODE_MITSUBISHI
	#if DECODE_LG
	if (d == LG)
	{
		if (0
			|| x == 0x003B
			|| x == 0x0001
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x003C
			|| x == 0x0002
			|| x == 0x0012
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x003D
			|| x == 0x0003
			|| x == 0x0013
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x003E
			|| x == 0x0014
			|| x == 0x0004
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x003F
			|| x == 0x0015
			|| x == 0x0045
			|| x == 0x0005
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0040
			|| x == 0x0046
			|| x == 0x0006
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0041
			|| x == 0x0047
			|| x == 0x0007
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0042
			|| x == 0x0018
			|| x == 0x0008
			|| x == 0x0048
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0043
			|| x == 0x0009
			|| x == 0x0019
			|| x == 0x0049
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0044
			|| x == 0x004B
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x0076
			|| x == 0x00F9
			|| x == 0x001E
			|| x == 0x0030
			|| x == 0x00D1
			|| x == 0x000A
			|| x == 0x001C
			|| x == 0x00C5
			|| x == 0x001F
			|| x == 0x000D
			|| x == 0x000C
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0010
			|| x == 0x0017
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0011
			|| x == 0x0016
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x001A
			|| x == 0x0072
			|| x == 0x0021
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0071
			|| x == 0x0020
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
	}
	#endif // DECODE_LG
	#if DECODE_SAMSUNG
	if (d == SAMSUNG)
	{
		if (0
			|| x == 0x0099
			|| x == 0x000F
			|| x == 0x0002
			|| x == 0x0091
			|| x == 0x0048
			|| x == 0x0014
			|| x == 0x0017
			|| x == 0x0019
			|| x == 0x0039
			|| x == 0x0024
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0004
			|| x == 0x0081
			|| x == 0x0001
			|| x == 0x0041
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0005
			|| x == 0x0082
			|| x == 0x0023
			|| x == 0x0042
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0006
			|| x == 0x0083
			|| x == 0x00AB
			|| x == 0x0003
			|| x == 0x0043
			|| x == 0x0026
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0007
			|| x == 0x0056
			|| x == 0x0020
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0084
			|| x == 0x0013
			|| x == 0x0044
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0009
			|| x == 0x0085
			|| x == 0x0045
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x000A
			|| x == 0x0086
			|| x == 0x0015
			|| x == 0x0046
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x000B
			|| x == 0x0055
			|| x == 0x003B
			|| x == 0x0030
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x000C
			|| x == 0x0087
			|| x == 0x0016
			|| x == 0x0047
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x000D
			|| x == 0x0088
			|| x == 0x003F
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x000E
			|| x == 0x0089
			|| x == 0x0018
			|| x == 0x0037
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0010
			|| x == 0x0051
			|| x == 0x002B
			|| x == 0x0033
			|| x == 0x0035
			|| x == 0x006C
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
		if (0
			|| x == 0x0011
			|| x == 0x0080
			|| x == 0x002F
			|| x == 0x001C
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x0012
			|| x == 0x0050
			|| x == 0x0032
			|| x == 0x0025
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
	}
	#endif // DECODE_SAMSUNG
	#if DECODE_SANYO
	if (d == SANYO)
	{
		if (0
			|| x == 0x0001
			|| x == 0x0010
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0002
			|| x == 0x0011
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0003
			|| x == 0x0009
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0004
			|| x == 0x000A
			|| x == 0x000F
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x0005
			|| x == 0x0014
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0006
			|| x == 0x0015
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0007
			|| x == 0x0016
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0017
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x000B
			|| x == 0x000E
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0018
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0012
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0013
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0019
		) {
			return TVREMOTE_KEY_0;
		}
	}
	#endif // DECODE_SANYO
	#if DECODE_SONY
	if (d == SONY)
	{
		if (0
			|| x == 0x0001
			|| x == 0x0041
			|| x == 0x001D
			|| x == 0x0022
			|| x == 0x000B
			|| x == 0x004B
			|| x == 0x0062
		) {
			return TVREMOTE_KEY_2;
		}
		if (0
			|| x == 0x0002
			|| x == 0x0042
			|| x == 0x0023
			|| x == 0x000C
			|| x == 0x004C
			|| x == 0x0063
		) {
			return TVREMOTE_KEY_3;
		}
		if (0
			|| x == 0x0003
			|| x == 0x0039
			|| x == 0x000D
			|| x == 0x0043
			|| x == 0x004D
			|| x == 0x0064
		) {
			return TVREMOTE_KEY_4;
		}
		if (0
			|| x == 0x0004
			|| x == 0x004E
			|| x == 0x0025
			|| x == 0x0044
			|| x == 0x000E
			|| x == 0x0065
		) {
			return TVREMOTE_KEY_5;
		}
		if (0
			|| x == 0x0005
			|| x == 0x0026
			|| x == 0x003B
			|| x == 0x0045
			|| x == 0x000F
			|| x == 0x0066
		) {
			return TVREMOTE_KEY_6;
		}
		if (0
			|| x == 0x0006
			|| x == 0x0010
			|| x == 0x0046
			|| x == 0x0067
		) {
			return TVREMOTE_KEY_7;
		}
		if (0
			|| x == 0x0007
			|| x == 0x0011
			|| x == 0x0047
			|| x == 0x0068
		) {
			return TVREMOTE_KEY_8;
		}
		if (0
			|| x == 0x0008
			|| x == 0x0017
			|| x == 0x0038
			|| x == 0x0016
			|| x == 0x0048
			|| x == 0x0069
		) {
			return TVREMOTE_KEY_9;
		}
		if (0
			|| x == 0x0012
			|| x == 0x001A
			|| x == 0x0054
		) {
			return TVREMOTE_KEY_VOLUMEUP;
		}
		if (0
			|| x == 0x0013
			|| x == 0x001E
			|| x == 0x004F
			|| x == 0x0055
		) {
			return TVREMOTE_KEY_VOLUMEDOWN;
		}
		if (0
			|| x == 0x0015
			|| x == 0x001F
			|| x == 0x003F
			|| x == 0x002F
			|| x == 0x0014
			|| x == 0x0037
			|| x == 0x0036
			|| x == 0x0024
			|| x == 0x005F
		) {
			return TVREMOTE_KEY_POWER;
		}
		if (0
			|| x == 0x0040
			|| x == 0x0021
			|| x == 0x0027
			|| x == 0x000A
			|| x == 0x004A
			|| x == 0x0061
		) {
			return TVREMOTE_KEY_1;
		}
		if (0
			|| x == 0x0049
			|| x == 0x0020
			|| x == 0x0009
			|| x == 0x0029
			|| x == 0x003E
			|| x == 0x0028
			|| x == 0x001C
			|| x == 0x0051
			|| x == 0x0060
		) {
			return TVREMOTE_KEY_0;
		}
		if (0
			|| x == 0x0058
			|| x == 0x0031
		) {
			return TVREMOTE_KEY_CHANNELUP;
		}
		if (0
			|| x == 0x003A
			|| x == 0x0030
		) {
			return TVREMOTE_KEY_CHANNELDOWN;
		}
	}
	#endif // DECODE_SONY
	return TVREMOTE_KEY_UNKNOWN;
}
