#include "network.h"
#include "wifi.h"       

bool network_is_ready(void)
{
    EventGroupHandle_t eg = wifi_get_event_group();
    if (eg == NULL)
        return false;

    return (xEventGroupGetBits(eg) & WIFI_CONNECTED_BIT);
}