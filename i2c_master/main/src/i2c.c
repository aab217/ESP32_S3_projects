#include "i2c.h"
#include "esp_log.h"

static const char *TAG = LOG_TAG_I2C;

i2c_master_bus_handle_t i2c_bus_handle = NULL;

esp_err_t i2c_bus_init(void)
{
    if (i2c_bus_handle != NULL) {
        ESP_LOGW(TAG, "I2C bus already initialized");
        return ESP_OK;
    }

    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &i2c_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "I2C bus initialized | SDA: GPIO%d | SCL: GPIO%d | Speed: %u Hz",
             SDA_PIN, SCL_PIN, I2C_SPEED_HZ);
    
    for (uint8_t addr = 0x01; addr < 0x3f; addr++) {
        esp_err_t ret = i2c_master_probe(i2c_bus_handle, addr, 100);
        if (ret == ESP_OK) {
            ESP_LOGI(LOG_TAG_MAIN, "Device found at address 0x%02X", addr);
        }
    }
    return ESP_OK;
}

esp_err_t i2c_add_device(uint8_t dev_addr, i2c_master_dev_handle_t *dev_handle_out)
{
    if (i2c_bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus not initialized yet!");
        return ESP_ERR_INVALID_STATE;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = I2C_SPEED_HZ,
    };

    esp_err_t ret = i2c_master_bus_add_device(i2c_bus_handle, &dev_config, dev_handle_out);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device 0x%02X: %s", dev_addr, esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Device added successfully at address 0x%02X", dev_addr);
    return ESP_OK;
}