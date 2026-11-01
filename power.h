#pragma once
#include <Arduino.h>

void power_init();
void power_sleep();
void power_wake();
float power_battery_voltage();
uint8_t power_battery_percent();
