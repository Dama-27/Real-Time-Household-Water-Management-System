#ifndef SOLENOID_CONTROL_H
#define SOLENOID_CONTROL_H

#include "driver/gpio.h"

void solenoid_init(gpio_num_t solenoidPin);
void setSolenoid(gpio_num_t solenoidPin, bool solenoidState);

#endif // SOLENOID_CONTROL_H
