// adc.c
#include "adc.h"

void ADC_Init(adc_oneshot_unit_handle_t *adc_handle)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id  = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, adc_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten    = ADC_ATTEN_DB_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, ADC_R_CHAN, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, ADC_G_CHAN, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, ADC_B_CHAN, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*adc_handle, ADC_A_CHAN, &chan_config));
}