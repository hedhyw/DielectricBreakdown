// ATmega48P
#include "main.h"

uint8_t ddr0 = 0;
uint8_t ddr1 = 0;
uint8_t port0 = 0;
uint8_t port1 = 0;


uint8_t maxVoltageHighEE EEMEM = 0;
uint8_t maxVoltageLowEE EEMEM = (MAX_VOLTAGE_TOP_LIMIT + MAX_VOLTAGE_LOW_LIMIT) / 2;
uint8_t voltageStepEE EEMEM = (VOLTAGE_STEP_LOW_LIMIT + VOLTAGE_STEP_TOP_LIMIT / 2);

enum STATE state = INITIAL;
uint16_t voltage = MIN_VOLTAGE;
uint16_t maxVoltage;
uint8_t voltageStep;
uint8_t displayTicks = 0;
uint16_t timer2OverflowsCount = 0;
volatile uint16_t delays = 0; // 0xFF ~ 1 second

uint8_t breakdownAlarmDisabled = 0;
uint8_t btnUpState = 0;
uint8_t btnDownState = 0;
uint8_t btnStartState = 0;
uint8_t testVoltageIndex = 0;

void uartSendByte(uint8_t byte) {
  while (!(UCSR0A & _BV(UDRE0)));
  UDR0 = byte;
}

void uartSendUINT16(uint16_t val) {
  uartSendByte(START_BYTE);
  uartSendByte(START_BYTE);
  uartSendByte(val >> 8);
  uartSendByte(val & 0xFF);
}

/* Configuration */

void readParameters() {
  maxVoltage = eeprom_read_byte(&maxVoltageLowEE);
  maxVoltage += eeprom_read_byte(&maxVoltageHighEE) << 8;
  voltageStep = eeprom_read_byte(&voltageStepEE);
}

void saveParametes() {
  eeprom_write_byte(&maxVoltageLowEE, maxVoltage);
  eeprom_write_byte(&maxVoltageHighEE, maxVoltage >> 8);
  eeprom_write_byte(&voltageStepEE, voltageStep);
}

void configureIO() {
  // Setup buttons.
  BTN_UP_DDR &= ~_BV(BTN_UP_BIT);
  BTN_DOWN_DDR &= ~_BV(BTN_DOWN_BIT);
  BTN_START_DDR &= ~_BV(BTN_START_BIT);
  // Setup lights.
  MAX_VOLTAGE_LIGHT_DDR  |= _BV(MAX_VOLTAGE_LIGHT_BIT);
  VOLTAGE_STEP_LIGHT_DDR |= _BV(VOLTAGE_STEP_LIGHT_BIT);
  END_LIGHT_LIGHT_DDR    |= _BV(END_LIGHT_BIT);
  BREAKDOWN_LIGHT_DDR    |= _BV(BREAKDOWN_LIGHT_BIT);
  // All lights are on.
  MAX_VOLTAGE_LIGHT_ON();
  VOLTAGE_STEP_LIGHT_ON();
  BREAKDOWN_LIGHT_ON();
  END_LIGHT_ON();
  // PWM on OC1A
  OC1A_DDR |= _BV(OC1A_PIN);
  // Hight voltage control
  HIGH_VOLTAGE_OFF();
  HIGH_VOLTAGE_CTL_DDR |= _BV(HIGH_VOLTAGE_CTL_BIT);
}

void configureUSART() {
  // Set baud rate
  UBRR0L = MYUBRR & 0xFF;
  UBRR0H = MYUBRR >> 8;
  // Transmitter Enable n
  UCSR0B |= _BV(TXEN0);
  // 8-bit, 1 stop bit
  UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

void configureTimer2() {
  // Disable interrupt.
  TIMSK2 = 0;
  // 1024 prescaler.
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
  // ~0xff overflows per second.
  TCNT2 = TIMER2_OFFSET;
  // Enable interrupt.
  TIMSK2 = _BV(TOIE2);
}

void configureTimer1() {
  OCR1A = 0;
  // Fast PWM, 10-bit
  // Clear OC1A/OC1B on Compare Match,
  // set OC1A/OC1B at BOTTOM (non-inverting mode)
  TCCR1A = _BV(WGM12) | _BV(WGM11) | _BV(WGM10) | _BV(COM1A1);
  // 1 - prescaler
  TCCR1B = _BV(CS10);
}


void configureADC() {
    // AV2CC with external capacitor at AREF pin
    // ADC0
    ADMUX = _BV(REFS0);
    // ADC Enable
    // ADC Start Conversionif (time-speedTimeC < 1/MAX_WHEEL_FREQUENCY) return;
    // ADC Auto Trigger Enable
    // 12MHz/128=93.750 kHz
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | \
             _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

/* Voltage controlling */

void resetVoltage() {
  HIGH_VOLTAGE_OFF();
  OCR1A = 0;
  uartSendUINT16(0);
}

void setVoltage(uint16_t voltage) {
  OCR1A = (voltage - MIN_VOLTAGE) * 1023.0 / MAX_VOLTAGE_TOP_LIMIT;
  HIGH_VOLTAGE_ON();
  uartSendUINT16(getRealValue(voltage));
}

void upVoltage() {
  voltage += voltageStep;
  if (voltage >= maxVoltage) {
    state = END;
    voltage = maxVoltage;
    resetVoltage();
  } else {
    setVoltage(voltage);
  }
}

/* Handlers */

void processLeds() {
  MAX_VOLTAGE_LIGHT_OFF();
  VOLTAGE_STEP_LIGHT_OFF();
  BREAKDOWN_LIGHT_OFF();
  END_LIGHT_OFF();
  switch (state) {
    case SETUP_MAX_VOLTAGE:
      MAX_VOLTAGE_LIGHT_ON();
      break;
    case SETUP_VOLTAGE_STEP:
      VOLTAGE_STEP_LIGHT_ON();
      break;
    case BREAKDOWN:
      BREAKDOWN_LIGHT_ON();
      break;
    case END:
      END_LIGHT_ON();
      break;
    case TEST_MODE:
      MAX_VOLTAGE_LIGHT_ON();
      VOLTAGE_STEP_LIGHT_ON();
      break;
    default:
      break;
  }
  if (breakdownAlarmDisabled) {
    BREAKDOWN_LIGHT_ON();
  }
}

void processDisplay() {
  switch(state) {
    case END:
    case BREAKDOWN:
    case PROCESSING:
      setLedValue(getRealValue(voltage));
      break;
    case TEST_MODE:
      setLedValue(voltage);
      break;
    case SETUP_VOLTAGE_STEP:
      setLedValue(voltageStep);
      break;
    case SETUP_MAX_VOLTAGE:
      setLedValue(maxVoltage);
      break;
    default:
      break;
  }
}

void processButtons() {
  if (BTN_START_PRESSED) {
    if (btnStartState == 0) {
      switch(state) {
        case SETUP_VOLTAGE_STEP:
          saveParametes();
        case BREAKDOWN:
          timer2OverflowsCount = 0;
          voltage = MIN_VOLTAGE;
          state = PROCESSING;
          break;
        case SETUP_MAX_VOLTAGE:
          state = SETUP_VOLTAGE_STEP;
          break;
        case TEST_MODE:
          voltage = testVoltages[testVoltageIndex];
          testVoltageIndex++;
          testVoltageIndex %= sizeof(testVoltages) / sizeof(uint16_t);
          if (voltage == 0) {
            resetVoltage();
          }
          else {
            setVoltage(voltage);
          }
        default:
          break;
      }
    }
    btnStartState = 1;
  } else {
    btnStartState = 0;
  }
  if (BTN_UP_PRESSED) {
    if (btnUpState == 0 || btnUpState == BUTTON_QUICK_TICKS) {
      uint8_t step = 1;
      if (btnUpState == BUTTON_QUICK_TICKS) step = BUTTON_QUICK_STEP;
      switch(state) {
        case SETUP_VOLTAGE_STEP:
          voltageStep += step;
          if (voltageStep > VOLTAGE_STEP_TOP_LIMIT) {
            voltageStep = VOLTAGE_STEP_LOW_LIMIT;
          }
          break;
        case SETUP_MAX_VOLTAGE:
          maxVoltage += step;
          if (maxVoltage > MAX_VOLTAGE_TOP_LIMIT)
            maxVoltage = MAX_VOLTAGE_LOW_LIMIT;
          break;
        default:
          break;
      }
    }
    if (++btnUpState > BUTTON_QUICK_TICKS) {
      btnUpState = BUTTON_QUICK_TICKS;
    }
  } else {
    btnUpState = 0;
  }
  if (BTN_DOWN_PRESSED) {
    if (btnDownState == 0 || btnDownState == BUTTON_QUICK_TICKS) {
      uint8_t step = 1;
      if (btnDownState == BUTTON_QUICK_TICKS) step = BUTTON_QUICK_STEP;
      switch(state) {
        case SETUP_VOLTAGE_STEP:
          voltageStep -= step;
          if (voltageStep < VOLTAGE_STEP_LOW_LIMIT || step > voltageStep) {
            voltageStep = VOLTAGE_STEP_TOP_LIMIT;
          }
          break;
        case SETUP_MAX_VOLTAGE:
          maxVoltage -= step;
          if (maxVoltage < MAX_VOLTAGE_LOW_LIMIT || step > maxVoltage) {
            maxVoltage = MAX_VOLTAGE_TOP_LIMIT;
          }
          break;
        default:
          break;
      }
    }
    if (++btnDownState > BUTTON_QUICK_TICKS) {
      btnDownState = BUTTON_QUICK_TICKS;
    }
  } else {
    btnDownState = 0;
  }
}

/* Interrupts */

ISR(TIMER2_OVF_vect) {
  ++delays;
  if (++timer2OverflowsCount == 0xFF && state == PROCESSING){
    timer2OverflowsCount = 0;
    upVoltage();
  }
  tickLedDisplay();
  putShiftByte(port1);
  putShiftByte(port0);
  saveShiftData();
  TCNT2 = TIMER2_OFFSET;
}

ISR(ADC_vect) {
  if (ADCW >= ADC_ALARM && !breakdownAlarmDisabled) {
    resetVoltage();
    if (state == PROCESSING) state = BREAKDOWN;
    else if (state != BREAKDOWN) {
      state = END;
      voltage = 999;
    }
  }
}

/* 0xFF is approximatly one second */
void delay(uint16_t ticks) {
  delays = 0;
  while (delays <= ticks) { }
}

int main(void) {
  readParameters();
  configureIO();
  configureShiftRegister();
  configureTimer1();
  configureTimer2();
  configureLedDisplay();
  configureADC();
  configureUSART();
  sei();
  delay(0xFF * 3);

  if (BTN_START_PRESSED) {
    maxVoltage = MAX_VOLTAGE_TOP_LIMIT;
    state = TEST_MODE;
  } else {
    state = SETUP_MAX_VOLTAGE;
  }

  if (BTN_UP_PRESSED) {
    breakdownAlarmDisabled = 1;
  }

  while(1) {
    processButtons();
    processLeds();
    processDisplay();
    delay(0xFF * 0.1);
  }
}
