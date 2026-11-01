#include "power.h"
#include "display.h"
#include <esp_sleep.h>

#define PIN_VSENSE 19
#define PIN_WAKE_PEN 1
#define PIN_WAKE_PWR 0

void power_init() {
  analogReadResolution(12);
  pinMode(PIN_VSENSE, INPUT);

  // Configure wakeups: Pen proximity (GPIO1) or Power button (GPIO0)
  esp_sleep_enable_ext1_wakeup((1ULL << PIN_WAKE_PEN) | (1ULL << PIN_WAKE_PWR), ESP_EXT1_WAKEUP_ANY_HIGH);
  Serial.println("[POWER] Power management initialized.");
}

float power_battery_voltage() {
  uint32_t sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += analogRead(PIN_VSENSE);
    delayMicroseconds(50);
  }
  float raw = (float)sum / 16.0f;
  float pin_v = (raw / 4095.0f) * 3.3f;
  // 100k / 100k divider -> multiply by 2.0
  return pin_v * 2.0f;
}

uint8_t power_battery_percent() {
  float v = power_battery_voltage();
  if (v >= 4.20f) return 100;
  if (v <= 3.30f) return 0;
  // Linear approximation of LiPo discharge
  return (uint8_t)(((v - 3.30f) / 0.90f) * 100.0f);
}

void power_sleep() {
  Serial.println("[POWER] Preparing for deep sleep...");
  display_power_gate(false);
  Serial.flush();
  esp_deep_sleep_start();
}

void power_wake() {
  display_power_gate(true);
}
