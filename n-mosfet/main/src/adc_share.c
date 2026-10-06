#include "adc_share.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "hal/adc_types.h"
#include <string.h>

static const char *TAG = "ADC_SHARE";

static SemaphoreHandle_t adc_mutex = NULL;
static adc_sample_t ping_pong_buffer[2];
static volatile uint8_t write_idx = 0;

static bool data_ready = false;

void adc_share_init(void)
{
    adc_mutex = xSemaphoreCreateMutex();
    if (adc_mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create mutex!");
    }
    memset(&ping_pong_buffer[0], 0, sizeof(adc_sample_t));
    memset(&ping_pong_buffer[1], 0, sizeof(adc_sample_t));
}

void adc_push_sample(uint8_t *buf, uint32_t rxlen)
{
    if (buf == NULL || rxlen == 0) return;

    adc_sample_t *active = &ping_pong_buffer[write_idx];

    memset(active, 0, sizeof(*active));

    active->timestamp_adc = esp_timer_get_time();

    size_t sample_count = rxlen / SOC_ADC_DIGI_RESULT_BYTES;

    if (sample_count > ADC_NUM_OF_CONV) {
        ESP_LOGW(TAG,
                 "Too many samples: %u, max: %u",
                 (unsigned)sample_count,
                 ADC_NUM_OF_CONV);

        sample_count = ADC_NUM_OF_CONV;
    }

    for (size_t i = 0; i < sample_count; i++) {

        adc_digi_output_data_t *p = (adc_digi_output_data_t *)&buf[i * SOC_ADC_DIGI_RESULT_BYTES];

        uint32_t ch = p->type2.channel;
        uint32_t val = p->type2.data;

        for (int chan = 0; chan < ADC_NUM_OF_CHAN; chan++) {

            if (ch == adc_channels[chan]) {

                active->data[chan][i] = val;
            }
        }
    }

    if (adc_mutex == NULL) {
        ESP_LOGE(TAG, "adc_mutex == NULL");
        return;
    }
    if (xSemaphoreTake(adc_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        write_idx ^= 1;
        data_ready = true;
        xSemaphoreGive(adc_mutex);
    } else {
        ESP_LOGW(TAG, "TX_mutex timeout");
    }
}

bool adc_get_latest(adc_sample_t *out)
{
    if (out == NULL || !data_ready) return false;

    if (xSemaphoreTake(adc_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        *out = ping_pong_buffer[!write_idx];
        data_ready = false;        
        xSemaphoreGive(adc_mutex);
        return true;      
    }
    else ESP_LOGW(TAG, "RX_mutex timeout");
    return false;
}
