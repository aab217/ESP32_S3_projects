#include "oled.h"
#include "i2c.h"
#include "config.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "font8x8.h"

static const char *TAG = "OLED";

static i2c_master_dev_handle_t oled_handle = NULL;

static uint8_t oled_buffer[8][128];

#define OLED_CMD  0x00
#define OLED_DATA 0x40

// ====================== Вспомогательные ======================
static esp_err_t oled_cmd(uint8_t cmd)
{
    uint8_t data[2] = {OLED_CMD, cmd};
    return i2c_master_transmit(oled_handle, data, 2, 100);
}

static esp_err_t oled_data_burst(const uint8_t *data, size_t len)
{
    uint8_t buf[129];
    buf[0] = OLED_DATA;
    memcpy(&buf[1], data, len);
    return i2c_master_transmit(oled_handle, buf, len + 1, 200);
}

// ====================== Инициализация ======================
esp_err_t oled_init(void)
{
    esp_err_t ret = i2c_add_device(OLED_I2C_ADDR, &oled_handle);
    if (ret != ESP_OK) return ret;

    vTaskDelay(pdMS_TO_TICKS(100));

    oled_cmd(0xAE);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x00);
    oled_cmd(0xA1);
    oled_cmd(0xC8);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0x81); oled_cmd(0xCF);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0xA4);
    oled_cmd(0xA6);
    oled_cmd(0xAF);

    oled_clear();
    ESP_LOGI(TAG, "OLED initialized (buffered)");
    return ESP_OK;
}

// ====================== Очистка буффера ======================
void oled_clear(void)
{
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

// ====================== Вывод строки ======================
static void draw_string(int x, int page, const char* str)
{
    if (page < 0 || page > 7) return;

    while (*str && x < 120) {
        uint8_t c = *str++;
        if (c < 32 || c > 127) c = '?';

        for (int i = 0; i < 8; i++) {
            if (x + i < 128)
                oled_buffer[page][x + i] = font8x8[c - 32][i];
        }
        x += 8;
    }
}
// ====================== Обновление дисплея ======================
void oled_refresh(void)
{
    for (int page = 0; page < 8; page++) {
        oled_cmd(0xB0 + page);   // set page
        oled_cmd(0x00);          // column low
        oled_cmd(0x10);          // column high

        oled_data_burst(oled_buffer[page], 128);
    }
}

// ====================== Вывод данных SCD40 ======================
void oled_show_data(int co2, float temp, float hum)
{
    oled_clear();

    char buf[20];

    snprintf(buf, sizeof(buf), "CO2: %4d", co2);
    draw_string(0, 0, buf);

    snprintf(buf, sizeof(buf), "Temp: %.2f C", temp);
    draw_string(0, 2, buf);

    snprintf(buf, sizeof(buf), "Hum:  %.2f %%", hum);
    draw_string(0, 4, buf);

    oled_refresh();        
}