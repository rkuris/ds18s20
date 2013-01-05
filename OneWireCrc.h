#ifndef OneWireCrc_h
#define OneWireCrc_h

#include <inttypes.h>

class OneWireCrc
{
	static uint8_t crc8( uint8_t *addr, uint8_t len);
};
#endif
