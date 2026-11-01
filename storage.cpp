#include "storage.h"
#include "display.h"
#include <SdFat.h>
#include <SPI.h>

#define PIN_SD_SCK  14
#define PIN_SD_MISO 13
#define PIN_SD_MOSI 15
#define PIN_SD_CS   16

static SPIClass sd_spi(HSPI);
static SdFat sd;
static bool sd_mounted = false;
static uint16_t current_page = 1;
static bool canvas_dirty = false;

void storage_init() {
  sd_spi.begin(PIN_SD_SCK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  if (!sd.begin(SdSpiConfig(PIN_SD_CS, DEDICATED_SPI, SD_SCK_MHZ(25), &sd_spi))) {
    Serial.println("[STORAGE] WARNING: SD card mount failed!");
    sd_mounted = false;
    return;
  }
  sd_mounted = true;
  Serial.println("[STORAGE] SD card mounted successfully (FAT32).");

  if (!sd.exists("/notes")) {
    sd.mkdir("/notes");
  }
  if (!sd.exists("/notes/nb_001")) {
    sd.mkdir("/notes/nb_001");
  }
}

static void get_page_path(uint16_t pageId, char* buf, size_t len) {
  snprintf(buf, len, "/notes/nb_001/page_%04u.epg", pageId);
}

bool storage_save_page(uint16_t pageId) {
  if (!sd_mounted) return false;
  uint8_t* fb = display_framebuffer();
  if (!fb) return false;

  char path[64];
  get_page_path(pageId, path, sizeof(path));

  File file = sd.open(path, O_WRITE | O_CREAT | O_TRUNC);
  if (!file) {
    Serial.printf("[STORAGE] Failed to open %s for writing\n", path);
    return false;
  }

  // Header: 16 bytes
  file.write((const uint8_t*)"EPG1", 4);
  uint16_t version = 1;
  uint16_t w = EPD_WIDTH;
  uint16_t h = EPD_HEIGHT;
  uint16_t dpi = 226;
  uint32_t ts = (uint32_t)millis();

  file.write((uint8_t*)&version, 2);
  file.write((uint8_t*)&w, 2);
  file.write((uint8_t*)&h, 2);
  file.write((uint8_t*)&dpi, 2);
  file.write((uint8_t*)&ts, 4);

  // Write raw bitmap payload
  size_t written = file.write(fb, FB_SIZE);
  file.close();

  canvas_dirty = false;
  Serial.printf("[STORAGE] Saved %s (%u bytes)\n", path, (unsigned int)written);
  return written == FB_SIZE;
}

bool storage_load_page(uint16_t pageId) {
  if (!sd_mounted) return false;
  uint8_t* fb = display_framebuffer();
  if (!fb) return false;

  char path[64];
  get_page_path(pageId, path, sizeof(path));

  File file = sd.open(path, O_READ);
  if (!file) {
    Serial.printf("[STORAGE] Page file %s not found\n", path);
    return false;
  }

  char magic[5] = {0};
  file.read((uint8_t*)magic, 4);
  if (strcmp(magic, "EPG1") != 0) {
    Serial.println("[STORAGE] Invalid file format magic!");
    file.close();
    return false;
  }

  // Skip header fields
  file.seek(16);
  size_t read_bytes = file.read(fb, FB_SIZE);
  file.close();

  Serial.printf("[STORAGE] Loaded %s (%u bytes)\n", path, (unsigned int)read_bytes);
  display_full_refresh();
  return read_bytes == FB_SIZE;
}

uint32_t storage_get_free_kb() {
  if (!sd_mounted) return 0;
  return sd.vol()->freeClusterCount() * sd.vol()->sectorsPerCluster() / 2;
}

void storage_task(void*) {
  for (;;) {
    if (canvas_dirty) {
      storage_save_page(current_page);
    }
    vTaskDelay(pdMS_TO_TICKS(30000)); // Auto-save interval: 30 seconds
  }
}
