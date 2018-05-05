#ifndef _BOOKWORM_CHECKSUM_H_
#define _BOOKWORM_CHECKSUM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t bookworm_fletcher16( uint8_t * data, int count );

#ifdef __cplusplus
}
#endif

#endif