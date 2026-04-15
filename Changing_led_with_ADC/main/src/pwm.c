#include "pwm.h"
#include "esp_err.h"

void PWM_Init(void)
{
    // Настройка таймера ШИМ
    ledc_timer_config_t timer_conf = {
        .speed_mode      = LEDC_MODE,
        .timer_num       = LEDC_TIMER,
        .freq_hz         = LEDC_FREQUENCY,
        .duty_resolution = LEDC_DUTY_RES,
        .clk_cfg         = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    // Настройка канала ШИМ
    ledc_channel_config_t channel_conf = {
        .gpio_num    = LED_PIN,
        .speed_mode  = LEDC_MODE,
        .channel     = LEDC_CHANNEL,
        .timer_sel   = LEDC_TIMER,
        .intr_type   = LEDC_INTR_DISABLE,
        .duty        = 0,
        .hpoint      = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));
}