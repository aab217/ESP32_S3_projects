#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {        
    uint32_t    time_us;  
    uint16_t    variable1;
    uint16_t    variable2;       
} UDP_Packet_t;

void udp_tx_task(void *pvParameters);
