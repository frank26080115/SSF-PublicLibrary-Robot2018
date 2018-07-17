#include "IRremote_Include.h"
#include "IRremoteInt.h"

//==============================================================================
//
//
//                              H E X B U G
//
//
//==============================================================================

#define BITS           8  // The number of bits in the command

#define HDR_MARK    1700  // The length of the Header:Mark
#define HDR_SPACE    320  // The lenght of the Header:Space

#define BIT_SPACE    320  // The length of a Bit:Space
#define ONE_MARK    1000  // The length of a Bit:Mark for 1's
#define ZERO_MARK    320  // The length of a Bit:Mark for 0's
#define AVG_MARK    ((ONE_MARK + ZERO_MARK) / 2)


//+=============================================================================
//
#if SEND_HEXBUG
void  IRsend::sendHexbug(unsigned long data, int nbits)
{
	// Set IR carrier frequency
	enableIROut(38);

	// Header
	mark (HDR_MARK);
	space(HDR_SPACE);

	// Data
	for (unsigned long  mask = 1UL << (nbits - 1);  mask;  mask >>= 1) {
		if (data & mask) {
			mark (ONE_MARK);
		} else {
			mark (ZERO_MARK);
		}
		space(BIT_SPACE);
	}

	// Footer
	space(0);  // Always end with the LED off
}
#endif

//+=============================================================================
//
#if DECODE_HEXBUG
bool  IRrecv::decodeHexbug(decode_results *results)
{
	unsigned long  data   = 0;  // Somewhere to build our code
	int            offset = 1;  // Skip the Gap reading
	int            i;

	// Check we have the right amount of data
	if (irparams.rawlen != 1 + 2 + (2 * BITS) + 1)  return false ;

	// Check initial Mark+Space match
	if (!MATCH_MARK (results->rawbuf[offset++], HDR_MARK ))  return false ;
	if (!MATCH_SPACE(results->rawbuf[offset++], HDR_SPACE))  return false ;

	// Read the bits in
	for (i = 0;  i < 32;  i++) {
		// Each bit looks like: MARK_1 + SPACE -> 1
		//                 or : MARK_0 + SPACE -> 0

		// IR data is big-endian, so we shuffle it in from the right:
		if      (MATCH_MARK(results->rawbuf[offset], ONE_MARK))   data = (data << 1) | 1 ;
		else if (MATCH_MARK(results->rawbuf[offset], ZERO_MARK))  data = (data << 1) | 0 ;
		else break ;
		offset++;
		if (!MATCH_SPACE(results->rawbuf[offset], BIT_SPACE)) break ;
		offset++;
	}

	// Success
	results->bits        = i;
	results->value       = data;
	results->decode_type = HEXBUG;
	return true;
}
#endif
