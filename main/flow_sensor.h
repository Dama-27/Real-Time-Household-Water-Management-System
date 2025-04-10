#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

typedef enum {
    FLOW_INPUT = 0,
    FLOW_OUTPUT = 1
} flow_sensor_type_t;

void init_flow_sensors();
float get_flow_rate_lpm(flow_sensor_type_t sensor_type);
float get_total_litres(flow_sensor_type_t sensor_type);

#endif // FLOW_SENSOR_H
