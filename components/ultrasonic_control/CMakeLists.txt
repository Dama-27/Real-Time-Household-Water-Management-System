idf_component_register(
    SRCS "ultrasonic_control.c"
    INCLUDE_DIRS "include"
    REQUIRES driver esp_system esp_timer
)