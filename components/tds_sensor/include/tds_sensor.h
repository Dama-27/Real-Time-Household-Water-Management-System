#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include "driver/adc.h"

void init_tds_sensor(adc_channel_t channel);
float read_tds(float temperature);

#endif
