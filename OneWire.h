#ifndef OneWire_h
#define OneWire_h

#include <inttypes.h>

class OneWire
{
  private:
    uint8_t m_pin;
    uint8_t port;
    uint8_t bitmask;
    uint8_t parasite;
    volatile uint8_t *outputReg;
    volatile uint8_t *inputReg;
    volatile uint8_t *modeReg;
    static const uint8_t RESET_RETRIES = 125;

  public:
    static const uint8_t CMDSKIP = 0xcc;
    OneWire( uint8_t pin, uint8_t parasite = true);
    
    // Perform a 1-Wire reset cycle. Returns 1 if a device responds
    // with a presence pulse.  Returns 0 if there is no device or the
    // bus is shorted or otherwise held low for more than 250uS
    uint8_t reset();
    uint8_t pin(void) { return m_pin; }

    // Issue a 1-Wire rom skip command, to address all on bus.
    void skip() { write(CMDSKIP); }

    // Write a byte. If 'power' is one then the wire is held high at
    // the end for parasitically powered devices. You are responsible
    // for eventually depowering it by calling depower() or doing
    // another read or write.
    void write( uint8_t v);

    // Read a byte.
    uint8_t read();

    // Write a bit. The bus is always left powered at the end, see
    // note in write() about that.
    void write_bit( uint8_t v);

    // Read a bit.
    uint8_t read_bit();

    // Stop forcing power onto the bus. You only need to do this if
    // you used the 'power' flag to write() or used a write_bit() call
    // and aren't about to do another read or write. You would rather
    // not leave this powered if you don't have to, just in case
    // someone shorts your bus.
    void depower();

};
#endif
