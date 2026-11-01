#pragma once
#include <Arduino.h>

void rasterizer_begin(uint16_t x, uint16_t y, uint16_t pressure, int8_t tx, int8_t ty, int tool);
void rasterizer_append(uint16_t x, uint16_t y, uint16_t pressure);
void rasterizer_end();
void render_task(void*);
