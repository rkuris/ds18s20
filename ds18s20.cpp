#include "ds18s20.h"
#include "PString.h"

int16_t ds18s20::getTempC(void)
{
    uint8_t rbuf[8];
    byte isds18s20 = 0;
    if ( !reset() )		// device not present?
        {
	isValid = false;
	return -32000;		// return very cold temperature (-2048C)
	}
    skip();			// select all devices on bus
    write(READ_SCRATCHPAD);	// ask to read scratchpad
    temp.temp = rbuf[0] = read();		// read lsb (byte 0)
    temp.temp += (int16_t)(rbuf[1] = read())*256;// read msb (byte 1)
    rbuf[2] = read();		// discard Th (byte 2)
    rbuf[3] = read();		// discard Tl (byte 3)
    if ( (rbuf[4]=read()) & 0x80 )// does config register have high bit set?
        {
	// oh great... a DS18S20 doesn't have a config register; always 0xFF
	// Please consider getting a DS18B20 instead
	temp.temp /= 2;		// chop the low bit
	temp.temp *= 16;		// in 16ths
        temp.temp -= 4;        // minus 0.25 per spec
	isds18s20 = 1;
	}
    rbuf[5] = read();		// discard reserved byte 5
    if ( isds18s20 )
	{
        temp.temp += 16 - (rbuf[6] = read());	// DS18S20: read fractions (byte 6)
	}
    else
    	rbuf[6] = read();	// DS18B20: discard reserved byte 6
    rbuf[7] = read();		// discard byte 7 (should be 0x10)
    if ( crc8(rbuf) != read() || rbuf[7] != 0x10 )
	{
	isValid = false;
	return -32000;
	}
    isValid = true;
    return temp.temp;
}

// This function eliminates "flutter" in the low bit by keeping track of the
// last temp.  It has to change by 1/8C before a change is indicated
int16_t ds18s20::getTempC(uint8_t &changed)
{
    getTempC();
    if ( !isValid )
        {
	// bad read, just return last value
	changed = false;
	return temp.temp;
	}
    switch ( abs(lasttemp.temp - temp.temp) )
    {
    case 0: case 1:
    	changed = false;
	break;
    case 2:
    	// special case; move saved temp up or down just 1/16
	lasttemp.temp += (temp.temp - lasttemp.temp)/2;
	changed = true;
	break;
    default:
	lasttemp.temp = temp.temp;
    	changed = true;
	break;
    }
    return temp.temp;
}
const char * const ds18s20::tempstr(int16_t t)
{
    static char strbuf[10];
    static char *fracs[17] = {"0","0625","125","1875","25","3125","375","4375","5","5625","625","6875","75","8125","875","9375","0"};
    PString output(strbuf, sizeof(strbuf));
    if (t<0)
    {
    	output.print('-');
    }
    output.print((int)abs(t)/16);
    output.print('.');
    if ( t < 0 )
    	output.print(fracs[16+t%16]);
    else
        output.print(fracs[t%16]);
    return strbuf;
}
const char * const ds18s20::tempstr(void)
{
    return ds18s20::tempstr(temp.temp);
}

const char * const ds18s20::tempstrF(void)
{
    // +2 assists with rounding
    return ds18s20::tempstr((temp.temp * 9 + 2)/ 5 + (32*16));
}
uint8_t ds18s20::crc8( uint8_t *addr )
{
    uint8_t i, j;
    uint8_t crc = 0;
    
    for (i = 0; i < 8; i++) {
        uint8_t inbyte = addr[i];
        for (j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
