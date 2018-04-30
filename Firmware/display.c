#include "display.h"

uint8_t ledSegmentNumber = 0;
uint8_t* ledValue;
uint8_t ledDigitsArray[11] = {DISPLAY_DIG_0, DISPLAY_DIG_1,
                              DISPLAY_DIG_2, DISPLAY_DIG_3,
                              DISPLAY_DIG_4, DISPLAY_DIG_5,
                              DISPLAY_DIG_6, DISPLAY_DIG_7,
                              DISPLAY_DIG_8, DISPLAY_DIG_9,
                              _BV(DISPLAY_G_BIT)};
uint8_t segmentsArray[SEGMENTS_COUNT] = {_BV(DISPLAY_SEGMENT0_BIT),
                                         _BV(DISPLAY_SEGMENT1_BIT),
                                         _BV(DISPLAY_SEGMENT2_BIT)};

void configureLedDisplay() {
  DISPLAY_DDR |= DISPLAY_DIG_8;
  ledValue = (uint8_t *) malloc(SEGMENTS_COUNT);
  uint8_t segmentsMask = 0;
  for (int i = 0; i < SEGMENTS_COUNT; ++i) {
    ledValue[i] = 10;
    segmentsMask |= segmentsArray[i];
  }
  DISPLAY_SEGMENTS_DDR |= segmentsMask;
}

void tickLedDisplay() {
  DISPLAY_SEGMENTS_OFF();
  DISPLAY_PORT |= DISPLAY_DIG_8; // Clear all.
  DISPLAY_SEGMENTS_PORT &= ~segmentsArray[ledSegmentNumber];
  DISPLAY_PORT &= ~ledDigitsArray[ledValue[ledSegmentNumber]];
  if (++ledSegmentNumber >= SEGMENTS_COUNT) ledSegmentNumber = 0;
}

void setLedValue(uint16_t val) {
  for (int i = SEGMENTS_COUNT - 1; i >= 0; --i) {
    if (val == 0) {
      ledValue[i] = 0;
    } else {
      ledValue[i] = val % 10;
      val /= 10;
    }
  }
}