#pragma once
#include <Arduino.h>

void storage_init();
void storage_task(void*);
bool storage_save_page(uint16_t pageId);
bool storage_load_page(uint16_t pageId);
uint32_t storage_get_free_kb();
