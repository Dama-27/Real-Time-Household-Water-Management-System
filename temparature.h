#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DS18B20_PIN GPIO_NUM_4

void writeBit(bool bitValue);
void writeByte(uint8_t data);
bool readBit();
uint8_t readByte();
bool resetSensor();
float getTemperature();

#endif // TEMPERATURE_H
