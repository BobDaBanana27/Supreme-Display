#pragma once
#include <Arduino.h>

struct PenEvent {
  uint16_t x, y;
  uint16_t pressure;
  int8_t tiltX, tiltY;
  bool penDown;
  bool button;
  uint32_t t;
};

enum Tool { TOOL_PEN, TOOL_ERASER, TOOL_HIGHLIGHTER, TOOL_SELECT };

void input_init();
void input_task(void*);
void render_task(void*);
void net_task(void*);
void storage_task(void*);
void ui_task(void*);
void digitizer_task(void*);

// stroke management
void begin_stroke(const PenEvent& e);
void append_point(const PenEvent& e);
void end_stroke();
Tool current_tool();
void set_tool(Tool t);
