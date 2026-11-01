#include "digitizer.h"
#include "calibration.h"
#include <SPI.h>

// EMR Digitizer interface over HSPI bus.
#define PIN_DIG_SCK   4
#define PIN_DIG_MISO  5
#define PIN_DIG_MOSI  3
#define PIN_DIG_CS    2
#define PIN_DIG_INT   1

#define DIG_SPI_SPEED 8000000UL // 8 MHz

static SPIClass dig_spi(HSPI);
static volatile bool new_sample_available = false;

static void IRAM_ATTR digitizer_isr() {
  new_sample_available = true;
}

void digitizer_init() {
  pinMode(PIN_DIG_CS, OUTPUT);
  digitalWrite(PIN_DIG_CS, HIGH);

  pinMode(PIN_DIG_INT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_DIG_INT), digitizer_isr, FALLING);

  dig_spi.begin(PIN_DIG_SCK, PIN_DIG_MISO, PIN_DIG_MOSI, PIN_DIG_CS);
  dig_spi.beginTransaction(SPISettings(DIG_SPI_SPEED, MSBFIRST, SPI_MODE0));

  Serial.println("[DIGITIZER] Initialized interrupt-driven EMR digitizer.");
}

static inline uint16_t read16() {
  uint16_t hi = dig_spi.transfer(0x00);
  uint16_t lo = dig_spi.transfer(0x00);
  return (hi << 8) | lo;
}

bool digitizer_read(PenEvent* out) {
  // Check interrupt pin or flag
  if (digitalRead(PIN_DIG_INT) == HIGH && !new_sample_available) {
    return false;
  }
  new_sample_available = false;

  digitalWrite(PIN_DIG_CS, LOW);
  uint8_t status = dig_spi.transfer(0x00);
  uint16_t rawX = read16();
  uint16_t rawY = read16();
  uint16_t pressure = read16();
  int8_t tx = (int8_t)dig_spi.transfer(0x00);
  int8_t ty = (int8_t)dig_spi.transfer(0x00);
  digitalWrite(PIN_DIG_CS, HIGH);

  bool penDown = (status & 0x01) && (pressure > 20);

  // Apply 6-parameter affine coordinate transformation
  uint16_t screenX = 0, screenY = 0;
  cal_transform(rawX, rawY, &screenX, &screenY);

  out->x = screenX;
  out->y = screenY;
  out->pressure = pressure;
  out->tiltX = tx;
  out->tiltY = ty;
  out->penDown = penDown;
  out->button = (status & 0x80) != 0;
  out->t = millis();

  return true;
}
