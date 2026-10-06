#pragma once

#include <stdint.h>
#include "adc_share.h"
#include "config.h"

#define ESP_SERVER_PORT     54321
#define CLIENT_PORT         12345

typedef struct __attribute__((packed)) {        
    uint64_t timestamp_adc;                                 // Время в микросекундах
    uint32_t data[ADC_NUM_OF_CHAN][ADC_NUM_OF_CONV];        // Значения ADC
} UDP_Packet_t;

void update_adc(UDP_Packet_t *pkt);

void udp_tx_task(void *pvParameters);
