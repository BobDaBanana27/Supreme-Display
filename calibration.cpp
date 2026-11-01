#include "calibration.h"
#include <Preferences.h>

static Preferences prefs;
// Default identity mapping
static float cal[6] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};

void cal_set(float a, float b, float c, float d, float e, float f) {
  cal[0] = a; cal[1] = b; cal[2] = c;
  cal[3] = d; cal[4] = e; cal[5] = f;
}

void cal_save() {
  prefs.begin("cal", false);
  prefs.putBytes("affine", cal, sizeof(cal));
  prefs.end();
  Serial.println("[CAL] Affine matrix saved to NVS.");
}

bool cal_load() {
  prefs.begin("cal", true);
  size_t n = prefs.getBytes("affine", cal, sizeof(cal));
  prefs.end();
  if (n == sizeof(cal)) {
    Serial.printf("[CAL] Loaded calibration: [%.2f, %.2f, %.2f | %.2f, %.2f, %.2f]\n",
      cal[0], cal[1], cal[2], cal[3], cal[4], cal[5]);
    return true;
  }
  Serial.println("[CAL] No saved calibration in NVS, using default identity.");
  return false;
}

void cal_transform(uint16_t rawX, uint16_t rawY, uint16_t* x, uint16_t* y) {
  float fx = cal[0] * rawX + cal[1] * rawY + cal[2];
  float fy = cal[3] * rawX + cal[4] * rawY + cal[5];
  *x = (uint16_t)constrain((int)fx, 0, 1871);
  *y = (uint16_t)constrain((int)fy, 0, 1403);
}
