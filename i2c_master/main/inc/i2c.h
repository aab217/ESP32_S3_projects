#pragma once

#include "esp_err.h"
#include "driver/i2c_master.h"
#include "config.h"

extern i2c_master_bus_handle_t i2c_bus_handle;

esp_err_t i2c_bus_init(void);
esp_err_t i2c_add_device(uint8_t dev_addr, i2c_master_dev_handle_t *dev_handle_out);