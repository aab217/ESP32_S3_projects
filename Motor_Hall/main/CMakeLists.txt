idf_component_register(
    SRCS 
        "main.c"
        "src/wifi.c"
        "src/network.c"
        "src/udp.c"
        "src/adc_share.c"
        "src/adc_driver.c"
        "src/pwm.c"

    INCLUDE_DIRS
        "inc"

    REQUIRES
        freertos
        esp_wifi
        esp_netif
        esp_event
        lwip
        nvs_flash
        esp_timer
        esp_adc
        esp_driver_ledc
        esp_driver_gpio
)