#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"

// ====================== I2C Настройки ======================
#define I2C_PORT        I2C_NUM_0
#define SDA_PIN         GPIO_NUM_1
#define SCL_PIN         GPIO_NUM_2
#define I2C_SPEED_HZ    100000U         

// ====================== SCD40 Настройки ======================
#define SCD40_I2C_ADDR  0x62

// ====================== OLED SSD1306 ======================
#define OLED_I2C_ADDR       0x3C
#define OLED_WIDTH          128
#define OLED_HEIGHT         64

// ====================== Общие настройки ======================
#define LOG_TAG_MAIN    "main"
#define LOG_TAG_I2C     "I2C_BUS"
#define LOG_TAG_OLED    "OLED"
#define LOG_TAG_SCD40   "SCD40"
