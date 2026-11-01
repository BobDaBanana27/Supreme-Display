#pragma once
#include <Arduino.h>

#define EPD_WIDTH  1872
#define EPD_HEIGHT 1404
#define FB_SIZE    (EPD_WIDTH * EPD_HEIGHT / 8) // ~328 KB 1bpp

void display_init();
void display_full_refresh();
void display_partial_refresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint8_t* display_framebuffer();
void display_clear(uint8_t color = 0xFF);
void display_power_gate(bool enable);
