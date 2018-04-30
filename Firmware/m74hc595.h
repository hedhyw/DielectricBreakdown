#ifndef __M74HC595_H__
#define __M74HC595_H__

  #include <stdint.h>
  #include <avr/io.h>
  #include "pins.h"

  void configureShiftRegister();
  void putShiftByte(uint8_t val);
  void saveShiftData();

#endif