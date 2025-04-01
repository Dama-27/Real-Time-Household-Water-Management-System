#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "driver/gpio.h"

// Function to initialize the relay pin
static inline void relay_init(gpio_num_t relayPin) {
    gpio_set_direction(relayPin, GPIO_MODE_OUTPUT);
}

// Function to control the relay state
static inline void setRelay(gpio_num_t relayPin, bool relayState) {
    gpio_set_level(relayPin, relayState ? 1 : 0);
}

#endif // RELAY_CONTROL_H
