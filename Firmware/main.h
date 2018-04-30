#ifndef __MAIN_H__
#define __MAIN_H__

  #define F_CPU 12000000UL

  #include <util/delay.h>
  #include "pins.h"
  #include "display.h"
  #include "calibration.h"
  #include "m74hc595.h"
  #include "glbl.h"
  #include <stdint.h>
  #include <stdio.h>
  #include <avr/io.h>
  #include <avr/eeprom.h>
  #include <avr/interrupt.h>

  // AMMETER_SHUNT_RESISTANCE * BREAKDOWN_CURRENT / 1000 < 5, V
  #define AMMETER_SHUNT_RESISTANCE 270e3 // Ohms
  #define BREAKDOWN_CURRENT 1e-5 // A
  #define SUPPLY_VOLTAGE 5.0 // V
  #define TIMER2_OFFSET (0xFF - F_CPU / 1024 / 0xFF) // 0xFF overflows per second
  #define ADC_ALARM (1024.0 * AMMETER_SHUNT_RESISTANCE * \
                     BREAKDOWN_CURRENT / (SUPPLY_VOLTAGE))

  #define VOLTAGE_STEP_TOP_LIMIT 19
  #define VOLTAGE_STEP_LOW_LIMIT 1
  #define MAX_VOLTAGE_TOP_LIMIT 380
  #define MAX_VOLTAGE_LOW_LIMIT 100
  #define MIN_VOLTAGE 60
  #define BUTTON_QUICK_TICKS 5
  #define BUTTON_QUICK_STEP 3

  /* UART */

  #define BAUD 9600
  #define MYUBRR (F_CPU/16/BAUD-1)
  #define START_BYTE 0xFF

  /* Sugar methods */

  #define HIGH_VOLTAGE_ON() (HIGH_VOLTAGE_CTL_PORT |= \
                                  _BV(HIGH_VOLTAGE_CTL_BIT))
  #define HIGH_VOLTAGE_OFF() (HIGH_VOLTAGE_CTL_PORT &= \
                                  ~_BV(HIGH_VOLTAGE_CTL_BIT))

  #define MAX_VOLTAGE_LIGHT_ON() (MAX_VOLTAGE_LIGHT_PORT |= \
                                    _BV(MAX_VOLTAGE_LIGHT_BIT))
  #define MAX_VOLTAGE_LIGHT_OFF() (MAX_VOLTAGE_LIGHT_PORT &= \
                                    ~_BV(MAX_VOLTAGE_LIGHT_BIT))
  #define VOLTAGE_STEP_LIGHT_ON() (VOLTAGE_STEP_LIGHT_PORT |= \
                                    _BV(VOLTAGE_STEP_LIGHT_BIT))
  #define VOLTAGE_STEP_LIGHT_OFF() (VOLTAGE_STEP_LIGHT_PORT &= \
                                    ~_BV(VOLTAGE_STEP_LIGHT_BIT))
  #define END_LIGHT_ON() (END_LIGHT_PORT |= _BV(END_LIGHT_BIT))
  #define END_LIGHT_OFF() (END_LIGHT_PORT &= ~_BV(END_LIGHT_BIT))
  #define BREAKDOWN_LIGHT_ON() (BREAKDOWN_LIGHT_PORT |= \
                                    _BV(BREAKDOWN_LIGHT_BIT))
  #define BREAKDOWN_LIGHT_OFF() (BREAKDOWN_LIGHT_PORT &= \
                                    ~_BV(BREAKDOWN_LIGHT_BIT))

  #define BTN_UP_PRESSED (BTN_UP_PIN & _BV(BTN_UP_BIT))
  #define BTN_DOWN_PRESSED (BTN_DOWN_PIN & _BV(BTN_DOWN_BIT))
  #define BTN_START_PRESSED (BTN_START_PIN & _BV(BTN_START_BIT))

  enum STATE {
    INITIAL,
    SETUP_MAX_VOLTAGE,
    SETUP_VOLTAGE_STEP,
    PROCESSING,
    BREAKDOWN,
    END,
    TEST_MODE
  };
#endif