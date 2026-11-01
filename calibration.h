#pragma once
#include <Arduino.h>

void cal_set(float a, float b, float c, float d, float e, float f);
void cal_save();
bool cal_load();
void cal_transform(uint16_t rawX, uint16_t rawY, uint16_t* x, uint16_t* y);
