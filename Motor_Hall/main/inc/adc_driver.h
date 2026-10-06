#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hal/adc_types.h"
#include "esp_adc/adc_continuous.h"

// ========== КОНФИГУРАЦИЯ ==========
#define ADC_SAMPLE_FREQ         500
#define ADC_FRAME_FREQ          ADC_SAMPLE_FREQ * ADC_NUM_OF_CHAN
#define ADC_MAX_STORE_BUF_SIZE  ADC_CONV_FRAME_SIZE * 4


void adc_measurement_task(void *pvParameters);