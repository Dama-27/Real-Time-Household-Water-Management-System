#ifndef ULTRASONIC_CONTROL_H
#define ULTRASONIC_CONTROL_H

#include "driver/gpio.h"
#include "esp_system.h"  // Include esp_system.h for general system functionality
#include "esp_timer.h"   // Include esp_timer.h for esp_timer_get_time
#include "esp_rom_sys.h" // Include esp_rom_sys.h for esp_rom_delay_us
#include <stdbool.h>

#define MAX_DISTANCE 400  // Max measurable distance in cm

void ultrasonic_init(gpio_num_t triggerPin, gpio_num_t echoPin);
float measure_distance(gpio_num_t triggerPin, gpio_num_t echoPin);

#endif // ULTRASONIC_CONTROL_H