#pragma once

#include <stdint.h>

#define ESP_SERVER_PORT     54321
#define CLIENT_PORT         12345

typedef struct __attribute__((packed)) {        
    uint32_t    time_us;  
    uint16_t    variable1;
    uint16_t    variable2;       
} UDP_Packet_t;

void udp_tx_task(void *pvParameters);
