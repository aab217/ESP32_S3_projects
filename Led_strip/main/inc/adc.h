#pragma once

#include "esp_adc/adc_oneshot.h"

#define ADC_R_CHAN    ADC_CHANNEL_3       // GPIO4
#define ADC_G_CHAN    ADC_CHANNEL_4       // GPIO5
#define ADC_B_CHAN    ADC_CHANNEL_5       // GPIO6
#define ADC_A_CHAN    ADC_CHANNEL_6       // GPIO7

#define ADC_ATTEN   ADC_ATTEN_DB_12     // 0..3.3V
#define ADC_BITWIDTH ADC_BITWIDTH_12    // 0..4095

void ADC_Init(adc_oneshot_unit_handle_t *out_handle);