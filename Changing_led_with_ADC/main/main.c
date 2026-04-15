#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "pwm.h"
#include "adc.h"

static adc_oneshot_unit_handle_t adc_handle = NULL;

void app_main(void)
{
    PWM_Init();
    ADC_Init(&adc_handle);

    int adc_raw = 0;

    printf("Programm started\n");

    while (1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHAN, &adc_raw));

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, adc_raw);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}