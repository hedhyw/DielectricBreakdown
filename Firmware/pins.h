#ifndef __PINS_H__
#define __PINS_H__

  #include <stdint.h>
  #include "glbl.h"

  /* Light */

  #define MAX_VOLTAGE_LIGHT_DDR (ddr1)
  #define MAX_VOLTAGE_LIGHT_PORT (port1)
  #define MAX_VOLTAGE_LIGHT_BIT (7)

  #define VOLTAGE_STEP_LIGHT_DDR (ddr1)
  #define VOLTAGE_STEP_LIGHT_PORT (port1)
  #define VOLTAGE_STEP_LIGHT_BIT (6)

  #define BREAKDOWN_LIGHT_DDR (ddr1)
  #define BREAKDOWN_LIGHT_PORT (port1)
  #define BREAKDOWN_LIGHT_BIT (5)

  #define END_LIGHT_LIGHT_DDR (ddr1)
  #define END_LIGHT_PORT (port1)
  #define END_LIGHT_BIT (4)

  /* Buttons */

  #define BTN_UP_DDR (DDRD)
  #define BTN_UP_PIN (PIND)
  #define BTN_UP_BIT (PD7)

  #define BTN_DOWN_DDR (DDRC)
  #define BTN_DOWN_PIN (PINC)
  #define BTN_DOWN_BIT (PC5)

  #define BTN_START_DDR (DDRC)
  #define BTN_START_PIN (PINC)
  #define BTN_START_BIT (PC4)

  /* Control */

  #define HIGH_VOLTAGE_CTL_DDR (DDRB)
  #define HIGH_VOLTAGE_CTL_PORT (PORTB)
  #define HIGH_VOLTAGE_CTL_BIT (PB0)

  /* Led display */

  //  *A*
  // F * B
  //  *G*
  // E * C
  //  *D* .
  #define DISPLAY_DDR (ddr0)
  #define DISPLAY_PORT (port0)
  #define DISPLAY_A_BIT (1)
  #define DISPLAY_B_BIT (2)
  #define DISPLAY_C_BIT (3)
  #define DISPLAY_D_BIT (4)
  #define DISPLAY_E_BIT (5)
  #define DISPLAY_F_BIT (6)
  #define DISPLAY_G_BIT (7)

  #define DISPLAY_SEGMENTS_DDR (ddr1)
  #define DISPLAY_SEGMENTS_PORT (port1)
  #define DISPLAY_SEGMENT0_BIT (3)
  #define DISPLAY_SEGMENT1_BIT (2)
  #define DISPLAY_SEGMENT2_BIT (1)

  #define SHIFT_REGISTER_DDR (DDRB)
  #define SHIFT_REGISTER_PORT (PORTB)
  #define SHIFT_REGISTER_CLK_BIT (PB5)
  #define SHIFT_REGISTER_DS_BIT (PB3)
  #define SHIFT_REGISTER_LATCH_BIT (PB2)

  #define OC1A_DDR (DDRB)
  #define OC1A_PIN (PB1)

#endif