#pragma once
#include <Arduino.h>
#include "input.h"

void digitizer_init();
bool digitizer_read(PenEvent* out);
