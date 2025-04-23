#include "solenoid_control.h"

void solenoid_init(gpio_num_t solenoidPin) {
    gpio_set_direction(solenoidPin, GPIO_MODE_OUTPUT);
}

void setSolenoid(gpio_num_t solenoidPin, bool solenoidState) {
    gpio_set_level(solenoidPin, solenoidState ? 1 : 0);
}
