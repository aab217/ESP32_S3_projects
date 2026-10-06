#include "wifi.h"
#include "config.h"

#include <string.h>

#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

static const char *TAG = "Wi-Fi";

static EventGroupHandle_t s_wifi_event_group;

/*
 * Wi-Fi event handler
 */
static void event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event =
            (wifi_event_ap_staconnected_t *)event_data;

        ESP_LOGW(
            TAG,
            "Station connected: " MACSTR " join, AID=%d",
            MAC2STR(event->mac),
            event->aid);

        xEventGroupSetBits(
            s_wifi_event_group,
            WIFI_CONNECTED_BIT);
    }

    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event =
            (wifi_event_ap_stadisconnected_t *)event_data;

        ESP_LOGW(
            TAG,
            "Station disconnected: " MACSTR " leave, AID=%d, reason=%d",
            MAC2STR(event->mac),
            event->aid,
            event->reason);

        xEventGroupClearBits(
            s_wifi_event_group,
            WIFI_CONNECTED_BIT);
    }
}

//Initialize ESP32 as Wi-Fi Access Point
void wifi_init_ap(void)
{

    // Create event group
    s_wifi_event_group = xEventGroupCreate();
    ESP_LOGI(TAG, "Created Event Group");

    // Create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_LOGI(TAG, "Created Event Loop");

    ESP_ERROR_CHECK(esp_netif_init());

    /*
     * Create default Wi-Fi AP network interface.
     *
     * Default configuration:
     *
     * IP      = 192.168.4.1
     * Gateway = 192.168.4.1
     * Mask    = 255.255.255.0
     *
     * DHCP server is enabled automatically.
     */
    esp_netif_create_default_wifi_ap();

    // Initialize Wi-Fi driver
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //Register Wi-Fi event handler
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &event_handler,
            NULL,
            NULL));

    // Configure Access Point
    wifi_config_t wifi_config =
        {
            .ap =
                {
                    .ssid = WIFI_AP_SSID,

                    .ssid_len = strlen(WIFI_AP_SSID),

                    .channel = WIFI_AP_CHANNEL,

                    .password = WIFI_AP_PASSWORD,

                    .max_connection = WIFI_AP_MAX_CONNECTIONS,

                    .authmode = WIFI_AUTH_WPA2_PSK,

                    .pmf_cfg =
                        {
                            .required = false,
                        },
                },
    };

    // If password is empty, use an open network.
    if (strlen(WIFI_AP_PASSWORD) == 0)  wifi_config.ap.authmode = WIFI_AUTH_OPEN;

    // Set AP mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // Apply AP configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    // Start Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    
    esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

    if (ap_netif != NULL)
    {
        esp_netif_ip_info_t ip_info;

        ESP_ERROR_CHECK(esp_netif_get_ip_info(ap_netif, &ip_info));

        ESP_LOGI(TAG, "AP IP: " IPSTR, IP2STR(&ip_info.ip));
        ESP_LOGI(TAG, "AP MASK: " IPSTR, IP2STR(&ip_info.netmask));
        ESP_LOGI(TAG, "AP GW: " IPSTR, IP2STR(&ip_info.gw));
    }
    
    ESP_LOGI(TAG, "Wi-Fi AP started");
    ESP_LOGI(TAG, "SSID: %s", WIFI_AP_SSID);
    ESP_LOGI(TAG, "Password: %s", WIFI_AP_PASSWORD);
    ESP_LOGI(TAG, "Channel: %d", WIFI_AP_CHANNEL);
    ESP_LOGI(TAG, "IP: 192.168.4.1");
}

// Check if at least one station is connected
bool wifi_is_connected(void)
{
    if (s_wifi_event_group == NULL) return false;
    return (xEventGroupGetBits(s_wifi_event_group) & WIFI_CONNECTED_BIT) != 0;
}

// Get Wi-Fi event group
EventGroupHandle_t wifi_get_event_group(void)
{
    return s_wifi_event_group;
}
