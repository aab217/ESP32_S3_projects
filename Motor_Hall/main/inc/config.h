#pragma once

#include "hal/adc_types.h"

/* _______________ ADC _______________*/
#define ADC_NUM_OF_CHAN         2

#define ADC_NUM_OF_CONV         32
#define ADC_CONV_FRAME_SIZE     ADC_NUM_OF_CONV * ADC_NUM_OF_CHAN * sizeof(uint32_t)

extern adc_channel_t adc_channels[ADC_NUM_OF_CHAN];

/* _______________ PWM _______________*/
#define PWM_PIN                 GPIO_NUM_42
#define DIR1_PIN                GPIO_NUM_1
#define DIR2_PIN                GPIO_NUM_2

/* _______________ UDP _______________*/