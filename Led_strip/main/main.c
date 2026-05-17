#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "pwm.h"
#include "adc.h"

static adc_oneshot_unit_handle_t adc_handle;

void app_main(void)
{
    PWM_Init();
    ADC_Init(&adc_handle);

    int adc_r = 0,
        adc_g = 0,
        adc_b = 0,
        adc_a = 0;

    uint32_t duty_r = 0,
             duty_g = 0,
             duty_b = 0;

    printf("Programm started\n");

while (1)
{

    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_A_CHAN, &adc_a));
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_R_CHAN, &adc_r));
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_G_CHAN, &adc_g));
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_B_CHAN, &adc_b));

    printf("A:%d  R:%d  G:%d  B:%d\n", adc_a, adc_r, adc_g, adc_b);

    // Яркость = (цвет * яркость / 4095)
    duty_r = (uint32_t)adc_r * adc_a / 4095;
    duty_g = (uint32_t)adc_g * adc_a / 4095;
    duty_b = (uint32_t)adc_b * adc_a / 4095;

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_R, duty_r);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_R);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_G, duty_g);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_G);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_B, duty_b);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_B);

    vTaskDelay(pdMS_TO_TICKS(5000));
}
}