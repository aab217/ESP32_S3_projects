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

    // Настройка красного канала ШИМ
    ledc_channel_config_t channel_r_conf = {
        .gpio_num    = R_LED_PIN,
        .speed_mode  = LEDC_MODE,
        .channel     = LEDC_CHANNEL_R,
        .timer_sel   = LEDC_TIMER,
        .intr_type   = LEDC_INTR_DISABLE,
        .duty        = 0,
        .hpoint      = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_r_conf));

    // Настройка зеленого канала ШИМ
    ledc_channel_config_t channel_g_conf = {
        .gpio_num    = G_LED_PIN,
        .speed_mode  = LEDC_MODE,
        .channel     = LEDC_CHANNEL_G,
        .timer_sel   = LEDC_TIMER,
        .intr_type   = LEDC_INTR_DISABLE,
        .duty        = 0,
        .hpoint      = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_g_conf));

    // Настройка синего канала ШИМ
    ledc_channel_config_t channel_b_conf = {
        .gpio_num    = B_LED_PIN,
        .speed_mode  = LEDC_MODE,
        .channel     = LEDC_CHANNEL_B,
        .timer_sel   = LEDC_TIMER,
        .intr_type   = LEDC_INTR_DISABLE,
        .duty        = 0,
        .hpoint      = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_b_conf));
}