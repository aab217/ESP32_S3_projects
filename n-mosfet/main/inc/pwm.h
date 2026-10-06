#pragma once

#include "driver/ledc.h"
#include "driver/gpio.h"
              
#define LEDC_MODE           LEDC_LOW_SPEED_MODE     // На S3 только низкоскоростной режим
#define LEDC_TIMER          LEDC_TIMER_0            // Первый таймер
#define LEDC_DUTY_RES       LEDC_TIMER_10_BIT       // 0..1023 (10 бит)
#define LEDC_FREQUENCY      125                   // 100 kHz
#define LEDC_CHANNEL        LEDC_CHANNEL_0          // Первый канал

void PWM_init(void);

