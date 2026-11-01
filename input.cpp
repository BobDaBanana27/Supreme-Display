#include "input.h"
#include "digitizer.h"
#include "display.h"
#include "rasterizer.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

static QueueHandle_t evq;
static Tool tool = TOOL_PEN;
static bool drawing = false;

void input_init() {
  evq = xQueueCreate(256, sizeof(PenEvent));
}

void digitizer_task(void*) {
  PenEvent e;
  for (;;) {
    if (digitizer_read(&e)) {
      xQueueSend(evq, &e, 0);
    }
    vTaskDelay(pdMS_TO_TICKS(4)); // ~250 Hz poll rate
  }
}

void input_task(void*) {
  PenEvent e;
  for (;;) {
    if (xQueueReceive(evq, &e, portMAX_DELAY)) {
      if (e.penDown && !drawing) {
        begin_stroke(e);
        drawing = true;
      } else if (e.penDown && drawing) {
        append_point(e);
      } else if (!e.penDown && drawing) {
        end_stroke();
        drawing = false;
      }
    }
  }
}

void begin_stroke(const PenEvent& e) {
  rasterizer_begin(e.x, e.y, e.pressure, e.tiltX, e.tiltY, tool);
}

void append_point(const PenEvent& e) {
  rasterizer_append(e.x, e.y, e.pressure);
}

void end_stroke() {
  rasterizer_end();
}

Tool current_tool() { return tool; }
void set_tool(Tool t) { tool = t; }
