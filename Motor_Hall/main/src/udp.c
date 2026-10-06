#include "udp.h"
#include "config.h"
#include "private.h"
#include "network.h"
#include "adc_share.h"  

#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "UDP";

void udp_tx_task(void *pvParameters)
{
    int sockfd = -1;
    struct sockaddr_in servaddr, cliaddr;
    UDP_Packet_t pkt = {0};
    
    while (1)
    {
        while (!network_is_ready())
        {
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        ESP_LOGI(TAG, "Network ready");

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
        {
            ESP_LOGE(TAG, "Socket error");
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        cliaddr.sin_family = AF_INET;
        cliaddr.sin_addr.s_addr = INADDR_ANY;
        cliaddr.sin_port = htons(CLIENT_PORT);

        if (bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
        {
            ESP_LOGE(TAG, "Bind error");
            close(sockfd);
            sockfd = -1;
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ESP_SERVER_IP);
        servaddr.sin_port = htons(ESP_SERVER_PORT);

        while (network_is_ready())
        {
            update_adc(&pkt);
            sendto(sockfd, &pkt, sizeof(pkt), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
            //ESP_LOGI(TAG, "Packet sent, timestamp: %llu", pkt.timestamp_adc);
            vTaskDelay(pdMS_TO_TICKS(2));
        }
        
        ESP_LOGW(TAG, "WiFi disconnected");
        close(sockfd);
        sockfd = -1;
    }
}

void update_adc(UDP_Packet_t *packet)
{
    if (packet == NULL) return;

    adc_sample_t latest;
    if (adc_get_latest(&latest)) {
        packet->timestamp_adc = latest.timestamp_adc;
        memcpy(packet->data, latest.data, sizeof(packet->data));
    }
}