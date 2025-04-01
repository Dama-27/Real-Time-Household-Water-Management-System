// solenoid.h
#ifndef SOLENOID_H
#define SOLENOID_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define inVALVE_PIN GPIO_NUM_26
#define outVALVE_PIN GPIO_NUM_27

void initSolenoid();
void controlSolenoid(bool state);

#endif // SOLENOID_H