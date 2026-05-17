#include "esp_log.h"
#include "config.h"
#include "i2c.h"
#include "scd40.h"
#include "oled.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    ESP_LOGI(LOG_TAG_MAIN, "Starting SCD40 project");

    ESP_ERROR_CHECK(i2c_bus_init());
    ESP_ERROR_CHECK(scd40_init());
    ESP_ERROR_CHECK(oled_init());

    scd40_data_t sensor_data;

    while (1)
    {        
        vTaskDelay(pdMS_TO_TICKS(5000));
        if (scd40_read(&sensor_data) == ESP_OK && sensor_data.valid) {
            oled_show_data(sensor_data.co2, sensor_data.temperature, sensor_data.humidity);
        } else {
            ESP_LOGW(LOG_TAG_MAIN, "Failed to read SCD40 data");
        }
    }
}