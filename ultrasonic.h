// ultrasonic.h
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include <stdio.h>

#define TRIG_PIN GPIO_NUM_5
#define ECHO_PIN GPIO_NUM_18

void initUltrasonic();
float getDistance();

#endif // ULTRASONIC_H