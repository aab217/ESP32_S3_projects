#pragma once

#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"

#define RECONNECT_DELAY_MS 10000
#define MAX_RETRY          5

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_init_sta(void);
EventGroupHandle_t wifi_get_event_group(void);