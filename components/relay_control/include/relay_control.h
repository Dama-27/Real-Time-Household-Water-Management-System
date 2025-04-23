#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "driver/gpio.h"
#include <stdbool.h>

void relay_init(gpio_num_t relayPin);
void setRelay(gpio_num_t relayPin, bool relayState);

#endif // RELAY_CONTROL_H
