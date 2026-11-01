// ============================================================================
// Supreme Display (EinkPad) - Standalone Arduino Firmware Entry Point
// Target: ESP32-S3 (N16R8, 8MB Octal PSRAM) + Waveshare 10.3" IT8951 + EMR Stylus
// Hack Club Half Life 2 Project Submission: BobDaBanana27/supreme-Display
// ============================================================================

#include "display.h"
#include "digitizer.h"
#include "calibration.h"
#include "input.h"
#include "rasterizer.h"
#include "storage.h"
#include "ui.h"
#include "power.h"
#include "sync.h"

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("============================================="));
  Serial.println(F("   Supreme Display - DIY E-Paper Tablet      "));
  Serial.println(F("   Hack Club Half Life 2 Submission          "));
  Serial.println(F("============================================="));

  power_init();
  cal_load();
  display_init();
  digitizer_init();
  storage_init();
  ui_init();
  input_init();
  sync_init();

  // Pin real-time input & rasterizer to Core 1
  xTaskCreatePinnedToCore(digitizer_task, "digitizer", 4096, nullptr, 5, nullptr, 1);
  xTaskCreatePinnedToCore(input_task,     "input",     8192, nullptr, 4, nullptr, 1);
  xTaskCreatePinnedToCore(render_task,    "render",    8192, nullptr, 3, nullptr, 1);

  // Pin background network & filesystem tasks to Core 0
  xTaskCreatePinnedToCore(storage_task,   "storage",   4096, nullptr, 2, nullptr, 0);
  xTaskCreatePinnedToCore(ui_task,        "ui",        4096, nullptr, 2, nullptr, 0);
  xTaskCreatePinnedToCore(sync_task,      "sync",      8192, nullptr, 1, nullptr, 0);

  Serial.println(F("[SYSTEM] Running. Ready for pen strokes!"));
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}
