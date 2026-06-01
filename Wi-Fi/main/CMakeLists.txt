idf_component_register(
    SRCS 
        "main.c"
        "src/wifi.c"
        "src/network.c"
        "src/udp.c"

    INCLUDE_DIRS
        "inc"

    PRIV_REQUIRES 
        freertos
        esp_wifi
        esp_netif
        esp_event
        lwip
        nvs_flash   
        esp_timer             
    )
