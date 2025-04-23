idf_component_register(
    SRCS "temperature_sensor.c"
    INCLUDE_DIRS "include"
    REQUIRES esp32-ds18b20 esp32-owb
)
