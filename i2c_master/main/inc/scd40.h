#pragma once

#include "esp_err.h"

typedef struct {
    int   co2;
    float temperature;
    float humidity;
    bool  valid;
} scd40_data_t;

esp_err_t scd40_init(void);
esp_err_t scd40_read(scd40_data_t *data);