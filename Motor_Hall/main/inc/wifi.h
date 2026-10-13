#pragma once

#include <stdbool.h>

#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0

void wifi_init_ap(void);

bool wifi_is_connected(void);

EventGroupHandle_t wifi_get_event_group(void);
