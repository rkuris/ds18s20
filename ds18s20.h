#ifndef ds18s20_h
#define ds18s20_h

#include "OneWire.h"
#include <inttypes.h>
#include <avr/io.h>
#include "WProgram.h"

class ds18s20temp
{
public:
	ds18s20temp()
		: temp(-32000) {}
	int16_t temp;		// temp in 1/16C
};
class ds18s20 : public OneWire
{
private:
    static const uint8_t READ_SCRATCHPAD = 0xBE,
    			 START_CONVERSION = 0x44;
    uint8_t crc8( uint8_t *addr );
public:
    ds18s20temp	temp;
    ds18s20temp	lasttemp;
    bool isValid;
    ds18s20( uint8_t pinArg, uint8_t parasiteArg = true )
    	: OneWire(pinArg, parasiteArg)
	{}
    void startConv(void)
    	{ reset(); skip(); write(START_CONVERSION); }
    void waitConv(void)
    	{ delay(850); }
    int16_t getTempC(void);		// gets temp in 1/16C increments; may be negative
    int16_t getTempC(uint8_t &changed);	// same, but indicate if it changed
    int16_t tempF(void) { return temp.temp * 9 / 5 + 32*16; }
    const char * const tempstr(void);
    static const char * const tempstr(int16_t t);
    const char * const tempstrF(void);
};

#endif
