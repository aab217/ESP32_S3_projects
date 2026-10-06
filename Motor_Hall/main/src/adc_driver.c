#include "adc_driver.h"
#include "adc_share.h"
#include "config.h"
#include "esp_log.h"
#include "esp_adc/adc_continuous.h"
#include <string.h>

static const char *TAG = "ADC";

adc_channel_t adc_channels[ADC_NUM_OF_CHAN] = {
    ADC_CHANNEL_3,  // GPIO4
    ADC_CHANNEL_4   // GPIO5
};

TaskHandle_t cd_adc_task = NULL; 
adc_continuous_handle_t adc_handle = NULL;

static IRAM_ATTR bool cbk_func(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    if (cd_adc_task != NULL) vTaskNotifyGiveFromISR(cd_adc_task, &mustYield);
    return mustYield == pdTRUE;
}

static void adc_driver_init()
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = ADC_MAX_STORE_BUF_SIZE,
        .conv_frame_size = ADC_CONV_FRAME_SIZE ,
        .flags = {
            .flush_pool = true
        },
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_continuous_config_t dig_cfg = {
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .sample_freq_hz = ADC_FRAME_FREQ,
        .pattern_num = ADC_NUM_OF_CHAN,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX];

    for (int i = 0; i < ADC_NUM_OF_CHAN; i++) {
        adc_pattern[i].atten = ADC_ATTEN_DB_12;
        adc_pattern[i].channel = adc_channels[i];
        adc_pattern[i].unit = ADC_UNIT_1;
        adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].channel: %d", i, adc_pattern[i].channel);
    }

    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &dig_cfg));

    adc_continuous_evt_cbs_t cb_cfg = {
        .on_conv_done = cbk_func,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(adc_handle, &cb_cfg, NULL));
}

void adc_measurement_task(void *pvParameters)
{
    adc_driver_init();  // Инициализация драйвера АЦП
    
    adc_share_init();  // Инициализация шины данных АЦП

    cd_adc_task = xTaskGetCurrentTaskHandle();
    adc_continuous_start(adc_handle);

    uint8_t buf[256];
    uint32_t rxlen = 0;

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        adc_continuous_read(adc_handle, buf, sizeof(buf), &rxlen, 0);

        adc_push_sample(buf, rxlen);
    }
}