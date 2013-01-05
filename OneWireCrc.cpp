#include "OneWireCrc.h"

// Compute a Dallas Semiconductor 8 bit CRC directly. 
//
uint8_t OneWireCrc::crc8( uint8_t *addr, uint8_t len)
{
    uint8_t i, j;
    uint8_t crc = 0;
    
    for (i = 0; i < len; i++) {
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
