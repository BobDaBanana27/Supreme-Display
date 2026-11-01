#include "rasterizer.h"
#include "display.h"
#include "input.h"
#include <math.h>

#define CANVAS_W 1872
#define CANVAS_H 1404

static uint16_t lastX = 0, lastY = 0;
static int activeTool = TOOL_PEN;
static uint16_t dirtyX0 = 0, dirtyY0 = 0, dirtyX1 = 0, dirtyY1 = 0;
static bool hasDirty = false;

static void expandDirty(uint16_t x, uint16_t y, uint8_t r) {
  uint16_t minX = (x >= r) ? (x - r) : 0;
  uint16_t maxX = ((uint32_t)x + r < CANVAS_W) ? (x + r) : (CANVAS_W - 1);
  uint16_t minY = (y >= r) ? (y - r) : 0;
  uint16_t maxY = ((uint32_t)y + r < CANVAS_H) ? (y + r) : (CANVAS_H - 1);

  if (!hasDirty) {
    dirtyX0 = minX; dirtyX1 = maxX;
    dirtyY0 = minY; dirtyY1 = maxY;
    hasDirty = true;
    return;
  }
  if (minX < dirtyX0) dirtyX0 = minX;
  if (maxX > dirtyX1) dirtyX1 = maxX;
  if (minY < dirtyY0) dirtyY0 = minY;
  if (maxY > dirtyY1) dirtyY1 = maxY;
}

static void stamp_brush(uint16_t cx, uint16_t cy, uint8_t radius, bool erase) {
  uint8_t* fb = display_framebuffer();
  if (!fb) return;

  int r2 = radius * radius;
  for (int dy = -radius; dy <= radius; dy++) {
    int py = cy + dy;
    if (py < 0 || py >= CANVAS_H) continue;

    uint32_t row_start = (uint32_t)py * (CANVAS_W / 8);
    for (int dx = -radius; dx <= radius; dx++) {
      if (dx * dx + dy * dy > r2) continue;
      int px = cx + dx;
      if (px < 0 || px >= CANVAS_W) continue;

      size_t byte_idx = row_start + (px / 8);
      uint8_t bit_mask = 0x80 >> (px & 7);

      if (erase) {
        fb[byte_idx] |= bit_mask;  // 1 = White (Erase)
      } else {
        fb[byte_idx] &= ~bit_mask; // 0 = Black (Ink)
      }
    }
  }
  expandDirty(cx, cy, radius);
}

static void draw_interpolated_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t radius, bool erase) {
  int dx = abs((int)x1 - (int)x0);
  int dy = -abs((int)y1 - (int)y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx + dy;

  int currX = x0;
  int currY = y0;

  for (;;) {
    stamp_brush(currX, currY, radius, erase);
    if (currX == x1 && currY == y1) break;
    int e2 = 2 * err;
    if (e2 >= dy) { err += dy; currX += sx; }
    if (e2 <= dx) { err += dx; currY += sy; }
  }
}

static inline uint8_t compute_radius(uint16_t pressure, int tool) {
  if (tool == TOOL_ERASER) return 14;
  if (tool == TOOL_HIGHLIGHTER) return 8;
  // Pen: 1 to 5 pixel radius depending on pressure
  return (uint8_t)(1 + (pressure >> 10));
}

void rasterizer_begin(uint16_t x, uint16_t y, uint16_t pressure, int8_t tx, int8_t ty, int tool) {
  lastX = x;
  lastY = y;
  activeTool = tool;
  hasDirty = false;

  uint8_t r = compute_radius(pressure, activeTool);
  stamp_brush(x, y, r, activeTool == TOOL_ERASER);
}

void rasterizer_append(uint16_t x, uint16_t y, uint16_t pressure) {
  uint8_t r = compute_radius(pressure, activeTool);
  draw_interpolated_line(lastX, lastY, x, y, r, activeTool == TOOL_ERASER);
  lastX = x;
  lastY = y;
}

void rasterizer_end() {
  if (hasDirty) {
    uint16_t width = dirtyX1 - dirtyX0 + 1;
    uint16_t height = dirtyY1 - dirtyY0 + 1;
    display_partial_refresh(dirtyX0, dirtyY0, width, height);
    hasDirty = false;
  }
}

void render_task(void*) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(16)); // ~60 Hz render loop
  }
}
