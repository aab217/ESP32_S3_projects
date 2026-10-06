#include "adc_share.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "hal/adc_types.h"

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
    if (rxlen == 0) return;

    adc_sample_t *active = &ping_pong_buffer[write_idx];

    active->timestamp_adc = esp_timer_get_time();

    for (int conv = 0; conv < rxlen; conv += SOC_ADC_DIGI_RESULT_BYTES) {
        adc_digi_output_data_t *p = (adc_digi_output_data_t*)&buf[conv];
        
        uint32_t ch = p->type2.channel;
        uint32_t val = p->type2.data;

        for (int chan = 0; chan < ADC_NUM_OF_CHAN; chan++) {
            if (ch == adc_channels[chan]) {
                active->data[chan][conv / SOC_ADC_DIGI_RESULT_BYTES] = val;
                ESP_LOGI(TAG, "CHAN %d: %lu", chan, (unsigned long)active->data[chan][conv / SOC_ADC_DIGI_RESULT_BYTES]);
            }
        }
    }
    if (xSemaphoreTake(adc_mutex, pdMS_TO_TICKS(1)) == pdTRUE) {
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

    if (xSemaphoreTake(adc_mutex, pdMS_TO_TICKS(1)) == pdTRUE) {
        *out = ping_pong_buffer[!write_idx];
        data_ready = false;        
        xSemaphoreGive(adc_mutex);
        return true;      
    }
    else ESP_LOGW(TAG, "RX_mutex timeout");
    return false;
}
