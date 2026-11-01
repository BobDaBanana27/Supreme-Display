#include "diag_cli.h"
#include "display.h"
#include "power.h"
#include "storage.h"
#include "calibration.h"

static String cmd_buffer = "";

void diag_cli_init() {
  Serial.println("\n==========================================");
  Serial.println("  Supreme Display Diagnostic Console v1.0 ");
  Serial.println("  Type 'help' for available bench tests   ");
  Serial.println("==========================================\n");
  Serial.print("supreme> ");
}

static void handle_command(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "help") {
    Serial.println("Commands:");
    Serial.println("  status     - Show voltage, memory, and SD status");
    Serial.println("  disp_test  - Draw test pattern to e-paper");
    Serial.println("  disp_clear - Clear e-paper display to white");
    Serial.println("  batt       - Print battery voltage and percent");
    Serial.println("  cal_reset  - Reset affine calibration to identity");
  } else if (cmd == "status") {
    Serial.printf("Battery: %.2fV (%u%%)\n", power_battery_voltage(), power_battery_percent());
    Serial.printf("Free SD: %u KB\n", (unsigned int)storage_get_free_kb());
    Serial.printf("Free PSRAM: %u bytes\n", (unsigned int)ESP.getFreePsram());
    Serial.printf("Free Heap:  %u bytes\n", (unsigned int)ESP.getFreeHeap());
  } else if (cmd == "disp_test") {
    Serial.println("Executing display pattern test...");
    uint8_t* fb = display_framebuffer();
    if (fb) {
      for (int i = 0; i < FB_SIZE; i++) fb[i] = (i % 2 == 0) ? 0xAA : 0x55;
      display_full_refresh();
    }
  } else if (cmd == "disp_clear") {
    display_clear(0xFF);
  } else if (cmd == "batt") {
    Serial.printf("Battery: %.3f V (%d%%)\n", power_battery_voltage(), power_battery_percent());
  } else if (cmd == "cal_reset") {
    cal_set(1, 0, 0, 0, 1, 0);
    cal_save();
    Serial.println("Calibration reset to identity.");
  } else if (cmd.length() > 0) {
    Serial.println("Unknown command. Type 'help'.");
  }

  Serial.print("supreme> ");
}

void diag_cli_poll() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r' || c == '\n') {
      Serial.println();
      handle_command(cmd_buffer);
      cmd_buffer = "";
    } else {
      Serial.print(c);
      cmd_buffer += c;
    }
  }
}
