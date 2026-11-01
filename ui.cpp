#include "ui.h"
#include "input.h"
#include "power.h"
#include "display.h"
#include "lvgl.h"

static lv_obj_t* toolbar;
static lv_obj_t* page_label;
static lv_obj_t* batt_label;

static void on_btn_pen(lv_event_t* e) {
  set_tool(TOOL_PEN);
  Serial.println("[UI] Switched to PEN");
}

static void on_btn_eraser(lv_event_t* e) {
  set_tool(TOOL_ERASER);
  Serial.println("[UI] Switched to ERASER");
}

static void on_btn_clear(lv_event_t* e) {
  display_clear(0xFF);
  Serial.println("[UI] Canvas cleared");
}

void ui_init() {
  lv_init();
  lv_obj_t* scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

  // Bottom toolbar container
  toolbar = lv_obj_create(scr);
  lv_obj_set_size(toolbar, LV_PCT(90), 54);
  lv_obj_align(toolbar, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_set_flex_flow(toolbar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(toolbar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  lv_obj_t* btn_pen = lv_btn_create(toolbar);
  lv_obj_add_event_cb(btn_pen, on_btn_pen, LV_EVENT_CLICKED, nullptr);
  lv_obj_t* lbl_pen = lv_label_create(btn_pen);
  lv_label_set_text(lbl_pen, "Pen");

  lv_obj_t* btn_er = lv_btn_create(toolbar);
  lv_obj_add_event_cb(btn_er, on_btn_eraser, LV_EVENT_CLICKED, nullptr);
  lv_obj_t* lbl_er = lv_label_create(btn_er);
  lv_label_set_text(lbl_er, "Eraser");

  lv_obj_t* btn_clr = lv_btn_create(toolbar);
  lv_obj_add_event_cb(btn_clr, on_btn_clear, LV_EVENT_CLICKED, nullptr);
  lv_obj_t* lbl_clr = lv_label_create(btn_clr);
  lv_label_set_text(lbl_clr, "Clear");

  page_label = lv_label_create(toolbar);
  lv_label_set_text(page_label, "Pg 1");

  batt_label = lv_label_create(toolbar);
  lv_label_set_text(batt_label, "100%");

  Serial.println("[UI] Minimalist e-paper toolbar initialized.");
}

void ui_set_page_number(uint16_t num) {
  if (page_label) {
    char buf[16];
    snprintf(buf, sizeof(buf), "Pg %u", num);
    lv_label_set_text(page_label, buf);
  }
}

void ui_task(void*) {
  for (;;) {
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
