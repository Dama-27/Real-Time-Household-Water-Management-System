#include "relay_control.h"

void relay_init(gpio_num_t relayPin) {
    gpio_set_direction(relayPin, GPIO_MODE_OUTPUT);
}

void setRelay(gpio_num_t relayPin, bool relayState) {
    gpio_set_level(relayPin, relayState ? 1 : 0);
}
