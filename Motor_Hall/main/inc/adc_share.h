#pragma once

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "config.h"

typedef struct {
    uint64_t timestamp_adc;                                     // Время в микросекундах
    uint32_t data[ADC_NUM_OF_CHAN][ADC_NUM_OF_CONV];        // Значения каналов
} adc_sample_t;

void adc_share_init(void);

void adc_push_sample(uint8_t *buf, uint32_t rxlen);

bool adc_get_latest(adc_sample_t *out);