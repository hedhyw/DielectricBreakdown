#include "m74hc595.h"

void configureShiftRegister() {
  SHIFT_REGISTER_DDR |= _BV(SHIFT_REGISTER_CLK_BIT) | \
                        _BV(SHIFT_REGISTER_DS_BIT) | \
                        _BV(SHIFT_REGISTER_LATCH_BIT);
  SHIFT_REGISTER_PORT &= ~(_BV(SHIFT_REGISTER_CLK_BIT) | \
                           _BV(SHIFT_REGISTER_DS_BIT) | \
                           _BV(SHIFT_REGISTER_LATCH_BIT));
  // SPI Enable
  // Master/Slave Select
  SPCR = (1<<SPE) | (1<<MSTR);
}

void putShiftByte(uint8_t val) {
  SHIFT_REGISTER_PORT &= ~_BV(SHIFT_REGISTER_LATCH_BIT);
  SPDR = val;
  while (!(SPSR & (1<<SPIF)));
}

void saveShiftData() {
  SHIFT_REGISTER_PORT |= _BV(SHIFT_REGISTER_LATCH_BIT);
  SHIFT_REGISTER_PORT &= ~_BV(SHIFT_REGISTER_LATCH_BIT);
}