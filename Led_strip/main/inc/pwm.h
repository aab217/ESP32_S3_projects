#pragma once

#include "driver/ledc.h"
#include "driver/gpio.h"

#define R_LED_PIN           GPIO_NUM_1              // Красный светодиод
#define G_LED_PIN           GPIO_NUM_2              // Зеленый светодиод
#define B_LED_PIN           GPIO_NUM_3              // Синий светодиод
#define LEDC_MODE           LEDC_LOW_SPEED_MODE     // На S3 только низкоскоростной режим
#define LEDC_TIMER          LEDC_TIMER_0            // Первый таймер
#define LEDC_DUTY_RES       LEDC_TIMER_12_BIT       // 0..4095
#define LEDC_FREQUENCY      5000                    // 5 kHz
#define LEDC_CHANNEL_R      LEDC_CHANNEL_0          // Канал для красного светодиода
#define LEDC_CHANNEL_G      LEDC_CHANNEL_1          // Канал для зеленого светодиода
#define LEDC_CHANNEL_B      LEDC_CHANNEL_2          // Канал для синего светодиода

void PWM_Init(void);