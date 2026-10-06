#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "udp.h"
#include "adc_driver.h"
#include "adc_share.h"
#include "pwm.h"
#include "config.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MAIN";
extern TaskHandle_t cd_adc_task;

void app_main(void)
{
    // NVS
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }
    ESP_LOGI(TAG, "NVS done");

    // Wi-Fi
    wifi_init_sta();
    ESP_LOGI(TAG, "WiFi done");
    PWM_init();
    // Запуск задач
    xTaskCreatePinnedToCore(adc_measurement_task, "adc_task", 8192, NULL, 3, &cd_adc_task, 1);
    xTaskCreatePinnedToCore(udp_tx_task, "udp_tx_task", 8192, NULL, 2, NULL, 0);
    
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}