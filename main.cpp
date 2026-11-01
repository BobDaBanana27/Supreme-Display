#include <Arduino.h>
#include "display.h"
#include "digitizer.h"
#include "calibration.h"
#include "input.h"
#include "rasterizer.h"
#include "storage.h"
#include "ui.h"
#include "power.h"
#include "sync.h"
#include "diag_cli.h"

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n[SYSTEM] Supreme Display Booting (Diagnostics Enabled)...");

  power_init();
  cal_load();
  display_init();
  digitizer_init();
  storage_init();
  ui_init();
  input_init();
  sync_init();
  diag_cli_init();

  // Create FreeRTOS Tasks with Core Pinning
  xTaskCreatePinnedToCore(digitizer_task, "dig", 4096, nullptr, 5, nullptr, 1);
  xTaskCreatePinnedToCore(input_task,     "inp", 8192, nullptr, 4, nullptr, 1);
  xTaskCreatePinnedToCore(render_task,    "rnd", 8192, nullptr, 3, nullptr, 1);
  xTaskCreatePinnedToCore(storage_task,   "sto", 4096, nullptr, 2, nullptr, 0);
  xTaskCreatePinnedToCore(ui_task,        "ui",  4096, nullptr, 2, nullptr, 0);
  xTaskCreatePinnedToCore(sync_task,      "net", 8192, nullptr, 1, nullptr, 0);

  Serial.println("[SYSTEM] Boot complete. Ready.");
}

void loop() {
  diag_cli_poll();
  delay(10);
}
