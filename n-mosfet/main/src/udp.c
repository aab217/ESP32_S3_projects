#include "udp.h"
#include "config.h"
#include "network.h"
#include "adc_share.h"

#include "lwip/sockets.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <errno.h>

static const char *TAG = "UDP";

static UDP_Packet_t pkt = {0};

// void udp_tx_task(void *pvParameters)
// {
//     int sockfd = -1;
//     struct sockaddr_in servaddr, cliaddr;

//     while (1)
//     {
//         while (!network_is_ready())
//         {
//             vTaskDelay(pdMS_TO_TICKS(300));
//         }

//         ESP_LOGI(TAG, "Network ready");

//         if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
//         {
//             ESP_LOGE(TAG, "Socket error");
//             vTaskDelay(pdMS_TO_TICKS(500));
//             continue;
//         }

//         memset(&servaddr, 0, sizeof(servaddr));
//         memset(&cliaddr, 0, sizeof(cliaddr));

//         cliaddr.sin_family = AF_INET;
//         cliaddr.sin_addr.s_addr = INADDR_ANY;
//         cliaddr.sin_port = htons(CLIENT_PORT);

//         if (bind(sockfd, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
//         {
//             ESP_LOGE(TAG, "Bind error");
//             close(sockfd);
//             sockfd = -1;
//             vTaskDelay(pdMS_TO_TICKS(500));
//             continue;
//         }

//         servaddr.sin_family = AF_INET;
//         servaddr.sin_addr.s_addr = inet_addr(ESP_SERVER_IP);
//         servaddr.sin_port = htons(ESP_SERVER_PORT);

//         while (network_is_ready())
//         {
//             update_adc(&pkt);
//             int ret = sendto(
//                 sockfd,
//                 &pkt,
//                 sizeof(pkt),
//                 0,
//                 (const struct sockaddr *)&servaddr,
//                 sizeof(servaddr));

//             if (ret < 0) ESP_LOGE(TAG, "sendto failed: errno=%d", errno);
                
//             else ESP_LOGI(TAG, "UDP sent: %d bytes", ret);

//             // ESP_LOGI(TAG, "Packet sent, timestamp: %llu", pkt.timestamp_adc);
//             vTaskDelay(pdMS_TO_TICKS(50));
//         }

//         ESP_LOGW(TAG, "WiFi disconnected");
//         close(sockfd);
//         sockfd = -1;
//     }
// }
void udp_tx_task(void *pvParameters)
{
    int sockfd = -1;
    struct sockaddr_in servaddr;

    while (1)
    {
        /*
         * Ждём подключения клиента к ESP32 AP
         */
        while (!network_is_ready())
        {
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        ESP_LOGI(TAG, "Network ready");

        /*
         * Создаём UDP socket
         */
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

        if (sockfd < 0)
        {
            ESP_LOGE(TAG, "Socket error: errno=%d", errno);
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

        /*
         * Адрес компьютера
         */
        memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(ESP_SERVER_IP);
        servaddr.sin_port = htons(ESP_SERVER_PORT);

        ESP_LOGI(
            TAG,
            "UDP target: %s:%d",
            ESP_SERVER_IP,
            ESP_SERVER_PORT
        );

        uint32_t tx_count = 0;

        /*
         * Передаём, пока клиент подключён
         */
        while (network_is_ready())
        {
            update_adc(&pkt);

            int ret = sendto(
                sockfd,
                &pkt,
                sizeof(pkt),
                0,
                (const struct sockaddr *)&servaddr,
                sizeof(servaddr)
            );

            tx_count++;

            if (ret < 0)
            {
                ESP_LOGE(
                    TAG,
                    "UDP send FAILED: errno=%d, tx_count=%lu",
                    errno,
                    (unsigned long)tx_count
                );
            }
            else
            {
                /*
                 * Не спамим UART 20 раз в секунду.
                 * Показываем один раз в секунду.
                 */
                if ((tx_count % 20) == 0)
                {
                    ESP_LOGI(
                        TAG,
                        "UDP sent #%lu: %d bytes, timestamp=%llu",
                        (unsigned long)tx_count,
                        ret,
                        (unsigned long long)pkt.timestamp_adc
                    );
                }
            }

            vTaskDelay(pdMS_TO_TICKS(50));
        }

        /*
         * Клиент отключился
         */
        ESP_LOGW(
            TAG,
            "UDP TX LOOP STOPPED: network_is_ready() = false"
        );

        close(sockfd);
        sockfd = -1;

        ESP_LOGW(TAG, "UDP socket closed");
    }
}

void update_adc(UDP_Packet_t *packet)
{
    if (packet == NULL)
        return;

    adc_sample_t latest;
    if (adc_get_latest(&latest))
    {
        packet->timestamp_adc = latest.timestamp_adc;
        memcpy(packet->data, latest.data, sizeof(packet->data));
    }
}