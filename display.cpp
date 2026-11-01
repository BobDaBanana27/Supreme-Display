#include "display.h"
#include <SPI.h>

// IT8951-class controller (Waveshare 10.3" HAT) over FSPI bus.
// Resolution: 1872 x 1404, 1-bit per pixel = 328,548 bytes framebuffer in PSRAM.
#define PIN_EPD_SCK   12
#define PIN_EPD_MOSI  11
#define PIN_EPD_CS    10
#define PIN_EPD_DC     9
#define PIN_EPD_RST    8
#define PIN_EPD_BUSY   7
#define PIN_EPD_PWR    6

#define SPI_SPEED_HZ 24000000UL // 24 MHz SPI clock

static SPIClass epd_spi(FSPI);
static uint8_t* fb = nullptr;

static inline void cs_low()  { digitalWrite(PIN_EPD_CS, LOW); }
static inline void cs_high() { digitalWrite(PIN_EPD_CS, HIGH); }
static inline void dc_cmd()  { digitalWrite(PIN_EPD_DC, LOW); }
static inline void dc_data() { digitalWrite(PIN_EPD_DC, HIGH); }

static void wait_ready() {
  // IT8951 pulls BUSY low when working, high when ready
  uint32_t start = millis();
  while (digitalRead(PIN_EPD_BUSY) == LOW) {
    if (millis() - start > 5000) {
      Serial.println("[EPD] Busy timeout warning!");
      break;
    }
    delayMicroseconds(50);
  }
}

static void send_cmd(uint16_t cmd) {
  wait_ready();
  dc_cmd();
  cs_low();
  epd_spi.transfer16(0x6000); // Command preamble
  wait_ready();
  epd_spi.transfer16(cmd);
  cs_high();
}

static void send_data(uint16_t data) {
  wait_ready();
  dc_data();
  cs_low();
  epd_spi.transfer16(0x0000); // Data preamble
  wait_ready();
  epd_spi.transfer16(data);
  cs_high();
}

void display_power_gate(bool enable) {
  pinMode(PIN_EPD_PWR, OUTPUT);
  digitalWrite(PIN_EPD_PWR, enable ? HIGH : LOW);
  delay(enable ? 50 : 10);
}

void display_init() {
  display_power_gate(true);

  pinMode(PIN_EPD_CS, OUTPUT);
  pinMode(PIN_EPD_DC, OUTPUT);
  pinMode(PIN_EPD_RST, OUTPUT);
  pinMode(PIN_EPD_BUSY, INPUT);

  cs_high();
  dc_data();

  // Reset sequence
  digitalWrite(PIN_EPD_RST, HIGH);
  delay(10);
  digitalWrite(PIN_EPD_RST, LOW);
  delay(20);
  digitalWrite(PIN_EPD_RST, HIGH);
  delay(50);

  // Initialize FSPI bus
  epd_spi.begin(PIN_EPD_SCK, -1, PIN_EPD_MOSI, PIN_EPD_CS);
  epd_spi.beginTransaction(SPISettings(SPI_SPEED_HZ, MSBFIRST, SPI_MODE0));

  // Allocate 1bpp canvas in external PSRAM
  fb = (uint8_t*)ps_malloc(FB_SIZE);
  if (!fb) {
    Serial.println("[EPD] ERROR: Failed to allocate framebuffer in PSRAM!");
  } else {
    memset(fb, 0xFF, FB_SIZE); // 0xFF = white background
    Serial.printf("[EPD] Allocated %d bytes framebuffer in PSRAM at %p\n", FB_SIZE, fb);
  }

  // Wake controller: SYS_RUN
  send_cmd(0x0001); // SYS_RUN
  delay(20);

  // Set memory conversion mode and dimensions
  send_cmd(0x0037); // Set AMODE / VCOM
  send_data(0x0000);

  wait_ready();
  Serial.println("[EPD] Display initialized successfully.");
}

uint8_t* display_framebuffer() {
  return fb;
}

void display_clear(uint8_t color) {
  if (fb) memset(fb, color, FB_SIZE);
  display_full_refresh();
}

void display_full_refresh() {
  if (!fb) return;
  wait_ready();

  // Load full image area (0, 0, 1872, 1404)
  send_cmd(0x0020); // LOAD_IMG_AREA
  send_data(0x0000); // X0
  send_data(0x0000); // Y0
  send_data(EPD_WIDTH);
  send_data(EPD_HEIGHT);

  // Stream framebuffer pixels
  wait_ready();
  dc_data();
  cs_low();
  epd_spi.transfer16(0x0000);
  wait_ready();

  uint16_t* ptr16 = (uint16_t*)fb;
  size_t words = FB_SIZE / 2;
  for (size_t i = 0; i < words; i++) {
    epd_spi.transfer16(ptr16[i]);
  }
  cs_high();

  // Trigger GC16 full refresh mode (mode 2)
  send_cmd(0x0024); // DISPLAY_AREA
  send_data(0x0000);
  send_data(0x0000);
  send_data(EPD_WIDTH);
  send_data(EPD_HEIGHT);
  send_data(0x0002); // GC16 waveform for high contrast clear

  wait_ready();
}

void display_partial_refresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  if (!fb || w == 0 || h == 0) return;

  // Clamp bounds
  if (x >= EPD_WIDTH) x = EPD_WIDTH - 1;
  if (y >= EPD_HEIGHT) y = EPD_HEIGHT - 1;
  if (x + w > EPD_WIDTH) w = EPD_WIDTH - x;
  if (y + h > EPD_HEIGHT) h = EPD_HEIGHT - y;

  // Align x to 16-pixel boundary for IT8951 word transfers
  uint16_t x0_aligned = (x / 16) * 16;
  uint16_t x1_aligned = ((x + w + 15) / 16) * 16;
  uint16_t w_aligned = x1_aligned - x0_aligned;

  wait_ready();

  // Load partial image area
  send_cmd(0x0020); // LOAD_IMG_AREA
  send_data(x0_aligned);
  send_data(y);
  send_data(w_aligned);
  send_data(h);

  wait_ready();
  dc_data();
  cs_low();
  epd_spi.transfer16(0x0000);
  wait_ready();

  // Stream only the bounding box rows
  for (uint16_t r = 0; r < h; r++) {
    uint32_t row_offset = (uint32_t)(y + r) * (EPD_WIDTH / 8) + (x0_aligned / 8);
    uint16_t* row_ptr = (uint16_t*)(fb + row_offset);
    size_t words = w_aligned / 16;
    for (size_t i = 0; i < words; i++) {
      epd_spi.transfer16(row_ptr[i]);
    }
  }
  cs_high();

  // Trigger A2 fast partial refresh (mode 1)
  send_cmd(0x0024); // DISPLAY_AREA
  send_data(x0_aligned);
  send_data(y);
  send_data(w_aligned);
  send_data(h);
  send_data(0x0001); // A2 fast update waveform (~120ms)

  wait_ready();
}
