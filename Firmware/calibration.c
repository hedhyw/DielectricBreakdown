#include "calibration.h"

// uint16_t real[8] = {
//   58,
//   104,
//   160,
//   212,
//   262,
//   312,
//   358,
//   380
// };

uint16_t real[14] = {
  49,
  66,
  94,
  123,
  149,
  179,
  199,
  226,
  251,
  279,
  299,
  324,
  346,
  379
};

uint16_t testVoltages[14] = {
  60,
  75,
  100,
  125,
  150,
  175,
  200,
  225,
  250,
  275,
  300,
  325,
  350,
  379
};

uint16_t getRealValue(uint16_t requiredValue) {
  static int valsLen = sizeof(testVoltages)/sizeof(testVoltages[0]);
  uint8_t i = 0;
  for (i = valsLen - 1; i >= 0; i--) {
    if (testVoltages[i] <= requiredValue) break;
  }
  if (i < 0 || i - 1 >= valsLen) return requiredValue;
  float testVoltagesRange = testVoltages[i + 1] - testVoltages[i];
  float val = (requiredValue - testVoltages[i]) / testVoltagesRange;
  float realRange = real[i + 1] - real[i];
  return val * realRange + real[i];
}