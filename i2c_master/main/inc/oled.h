#pragma once

#include "esp_err.h"

esp_err_t oled_init(void);
void oled_clear(void);
void oled_refresh(void);


void oled_show_data(int co2, float temp, float hum);