#ifndef __DISPLAY_H__
#define __DISPLAY_H__

  #include "pins.h"
  #include <stdint.h>
  #include <stdlib.h>
  #include <avr/io.h>

  #define SEGMENTS_COUNT 3

  void tickLedDisplay();
  void configureLedDisplay();
  void setLedValue(uint16_t val);

  /* Sugar */

  #define DISPLAY_SEGMENTS_OFF() (DISPLAY_SEGMENTS_PORT |= \
                                  _BV(DISPLAY_SEGMENT0_BIT) | \
                                  _BV(DISPLAY_SEGMENT1_BIT) | \
                                  _BV(DISPLAY_SEGMENT2_BIT))

  //  *A*
  // F * B
  //  *G*
  // E * C
  //  *D* .
  #define DISPLAY_DIG_0 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_E_BIT) | _BV(DISPLAY_F_BIT))
  #define DISPLAY_DIG_1 (_BV(DISPLAY_B_BIT) | _BV(DISPLAY_C_BIT))
  #define DISPLAY_DIG_2 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_G_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_E_BIT))
  #define DISPLAY_DIG_3 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_G_BIT))
  #define DISPLAY_DIG_4 (_BV(DISPLAY_G_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_F_BIT))
  #define DISPLAY_DIG_5 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_G_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_F_BIT))
  #define DISPLAY_DIG_6 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_G_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_E_BIT) | _BV(DISPLAY_F_BIT))
  #define DISPLAY_DIG_7 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT))
  #define DISPLAY_DIG_8 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_E_BIT) | _BV(DISPLAY_F_BIT) | \
                         _BV(DISPLAY_G_BIT) )
  #define DISPLAY_DIG_9 (_BV(DISPLAY_A_BIT) | _BV(DISPLAY_B_BIT) | \
                         _BV(DISPLAY_C_BIT) | _BV(DISPLAY_D_BIT) | \
                         _BV(DISPLAY_G_BIT) | _BV(DISPLAY_F_BIT))
#endif