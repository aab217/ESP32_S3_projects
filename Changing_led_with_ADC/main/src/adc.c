#include "adc.h"
#include "esp_err.h"

void ADC_Init(adc_oneshot_unit_handle_t *out_handle)
{
    adc_oneshot_unit_handle_t adc1_handle;

    // Настройка ADC
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id  = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    // Настройка канала ADC
    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH,
        .atten    = ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHAN, &chan_config));

    *out_handle = adc1_handle;
}