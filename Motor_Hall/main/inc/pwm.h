#pragma once

#include "driver/ledc.h"
#include "driver/gpio.h"
              
#define LEDC_MODE           LEDC_LOW_SPEED_MODE     // На S3 только низкоскоростной режим
#define LEDC_TIMER          LEDC_TIMER_0            // Первый таймер
#define LEDC_DUTY_RES       LEDC_TIMER_12_BIT       // 0..4095
#define LEDC_FREQUENCY      100                     // 100 Hz
#define LEDC_CHANNEL        LEDC_CHANNEL_0          // Первый канал

void PWM_init(void);
void DIR_init(gpio_num_t dir_pin);
