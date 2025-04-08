idf_component_register(SRCS "Real-Time-Household-Water-Management-System.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp32-ds18b20 temperature_sensor tds_sensor relay_control solenoid_control ultrasonic_control flow_sensor)
