#ifndef SOLENOID_CONTROL_H
#define SOLENOID_CONTROL_H

#include "driver/gpio.h"

// Function to initialize the solenoid pin
static inline void solenoid_init(gpio_num_t solenoidPin) {
    gpio_set_direction(solenoidPin, GPIO_MODE_OUTPUT);
}

// Function to control the solenoid state
static inline void setSolenoid(gpio_num_t solenoidPin, bool solenoidState) {
    gpio_set_level(solenoidPin, solenoidState ? 1 : 0);
}

#endif // SOLENOID_CONTROL_H
