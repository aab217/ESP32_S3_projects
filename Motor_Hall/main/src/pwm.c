#include "pwm.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "config.h"

void PWM_init(void)
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
        .gpio_num    = PWM_PIN,
        .speed_mode  = LEDC_MODE,
        .channel     = LEDC_CHANNEL,
        .timer_sel   = LEDC_TIMER,
        .intr_type   = LEDC_INTR_DISABLE,
        .duty        = 1023,
        .hpoint      = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_conf));

    DIR_init(DIR1_PIN);
    gpio_set_level(DIR1_PIN, 0);

    DIR_init(DIR2_PIN);
    gpio_set_level(DIR2_PIN, 1);
}

void DIR_init(gpio_num_t dir_pin)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << dir_pin),    // выбираем GPIO
        .mode = GPIO_MODE_OUTPUT,             // режим: выход
        .pull_up_en = GPIO_PULLUP_DISABLE,    // отключаем подтяжку вверх
        .pull_down_en = GPIO_PULLDOWN_DISABLE,// отключаем подтяжку вниз
        .intr_type = GPIO_INTR_DISABLE        // прерывания не нужны
    };
    gpio_config(&io_conf);
}