#include "scd40.h"
#include "i2c.h"
#include "config.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = LOG_TAG_SCD40;
static i2c_master_dev_handle_t scd40_dev_handle = NULL;

// ====================== CRC8 ======================
static uint8_t scd40_crc8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
        }
    }
    return crc;
}

// ====================== Отправка команды ======================
static esp_err_t scd40_send_command(uint16_t cmd)
{
    uint8_t buf[2] = {(cmd >> 8) & 0xFF, cmd & 0xFF};
    return i2c_master_transmit(scd40_dev_handle, buf, 2, 200);
}

// ====================== Инициализация SCD40 ======================
esp_err_t scd40_init(void)
{
    esp_err_t ret = i2c_add_device(SCD40_I2C_ADDR, &scd40_dev_handle);
    if (ret != ESP_OK) {
        return ret;
    }

    ESP_LOGI(TAG, "SCD40 device added, starting initialization...");

    scd40_send_command(0x3F86);        // stop
    vTaskDelay(pdMS_TO_TICKS(550));    

    scd40_send_command(0x3646);        // reinit
    vTaskDelay(pdMS_TO_TICKS(200));

    // Wake up
    if (scd40_send_command(0x36F6) != ESP_OK) {
        ESP_LOGW(TAG, "wake_up returned expected NACK (normal)");
    }
    vTaskDelay(pdMS_TO_TICKS(50));

    // Запускаем periodic measurement
    ret = scd40_send_command(0x21B1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "start_periodic_measurement FAILED: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "start_periodic_measurement sent SUCCESSFULLY");
    vTaskDelay(pdMS_TO_TICKS(100));

    return ESP_OK;
}

// ====================== Чтение измерений ======================
esp_err_t scd40_read(scd40_data_t *data)
{
    if (data == NULL) return ESP_ERR_INVALID_ARG;

    uint8_t read_cmd[2] = {0xEC, 0x05};
    uint8_t buffer[9] = {0};

    esp_err_t ret = i2c_master_transmit_receive(scd40_dev_handle, read_cmd, 2, buffer, 9, 300);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read SCD40: %s", esp_err_to_name(ret));
        data->valid = false;
        return ret;
    }

    // Проверка CRC
    if (scd40_crc8(buffer, 2)     != buffer[2] ||
        scd40_crc8(buffer + 3, 2) != buffer[5] ||
        scd40_crc8(buffer + 6, 2) != buffer[8]) {
        ESP_LOGW(TAG, "CRC error");
        data->valid = false;
        return ESP_ERR_INVALID_CRC;
    }

    // Разбор данных
    uint16_t co2_raw  = (buffer[0] << 8) | buffer[1];
    uint16_t temp_raw = (buffer[3] << 8) | buffer[4];
    uint16_t hum_raw  = (buffer[6] << 8) | buffer[7];

    data->co2         = co2_raw;
    data->temperature = -45.0f + 175.0f * temp_raw / 65536.0f;
    data->humidity    = 100.0f * hum_raw / 65536.0f;
    data->valid       = true;

    // Вывод в консоль (оставляем для отладки)
    ESP_LOGI(TAG, "CO₂: %4d ppm   Temp: %.2f °C   Hum: %.1f %%", 
             data->co2, data->temperature, data->humidity);

    return ESP_OK;
}