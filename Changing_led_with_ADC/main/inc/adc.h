#pragma once

#include "esp_adc/adc_oneshot.h"

#define ADC_CHAN    ADC_CHANNEL_1       // GPIO2
#define ADC_ATTEN   ADC_ATTEN_DB_12     // 0..3.3V
#define ADC_BITWIDTH ADC_BITWIDTH_12    // 0..4095

void ADC_Init(adc_oneshot_unit_handle_t *out_handle);