#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "wifi.h"
#include "udp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "MAIN";

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }
    ESP_LOGI(TAG, "NSV_flash_init done");

    wifi_init_sta();
    ESP_LOGI(TAG, "Sta_init done");

    xTaskCreatePinnedToCore(udp_tx_task, "udp_tx_task", 4096, NULL, 5, NULL, 0);
    
    while(1)
    {
vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}
