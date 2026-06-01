#include "udp.h"
#include "config.h"
#include "private.h"
#include "network.h"

#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "math.h"

static const char *TAG = "UDP";

//-------------------------------------------------------------
void udp_tx_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    int sockfd = -1;
    struct sockaddr_in servaddr, cliaddr;

    UDP_Packet_t pkt = {0};

    while (1)
    {
        // Ждём, пока Wi-Fi подключён и IP получен
        while (!network_is_ready())
        {
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        ESP_LOGI(TAG, "Network is ready. Creating socket...");

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
        {
            ESP_LOGE(TAG, "socket not created");
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Заполнение информации о клиенте (bind)
        cliaddr.sin_family      = AF_INET;
        cliaddr.sin_addr.s_addr = INADDR_ANY;
        cliaddr.sin_port        = htons(CLIENT_PORT);

        if (bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(struct sockaddr_in)) < 0)
        {
            ESP_LOGE(TAG, "socket not binded");
            close(sockfd);
            sockfd = -1;
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        ESP_LOGI(TAG, "socket was binded");

        // Заполнение информации о сервере
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ESP_SERVER_IP);
        servaddr.sin_port        = htons(ESP_SERVER_PORT);

        xLastWakeTime = xTaskGetTickCount();
        
        // Основной цикл отправки
        while (network_is_ready())          
        {
            pkt.time_us = (uint32_t)esp_timer_get_time();
            pkt.variable1++;
            sendto(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
            ESP_LOGI(TAG, "Data sent: %u", pkt.time_us);\
            vTaskDelay(1 / portTICK_PERIOD_MS);
            //vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
        }

        // Если вышли — значит Wi-Fi отвалился
        ESP_LOGW(TAG, "WiFi disconnected → closing UDP socket");
        if (sockfd >= 0)
        {
            shutdown(sockfd, 0);
            close(sockfd);
            sockfd = -1;
        }
    }
}