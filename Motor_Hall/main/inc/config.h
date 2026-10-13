#pragma once

#include "hal/adc_types.h"

/* _______________ ADC _______________*/
#define ADC_NUM_OF_CHAN         1

#define ADC_NUM_OF_CONV         500
#define ADC_CONV_FRAME_SIZE     ADC_NUM_OF_CONV * ADC_NUM_OF_CHAN * sizeof(uint32_t) //500*1*4=2000 байт

extern adc_channel_t adc_channels[ADC_NUM_OF_CHAN];

//

/* _______________ PWM _______________*/
#define PWM_PIN                 GPIO_NUM_1

/* _______________ Wi-Fi _______________*/
#define WIFI_AP_SSID                    "ESP32_S3_AP"
#define WIFI_AP_PASSWORD                "12345679"
#define WIFI_AP_CHANNEL                 1
#define WIFI_AP_MAX_CONNECTIONS         1
#define ESP_SERVER_IP                   "192.168.4.2"