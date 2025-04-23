#include "ultrasonic_control.h"
#include "esp_system.h"  // Include esp_system.h for system-related functionality
#include "esp_rom_sys.h" // Include esp_rom_sys.h for esp_rom_delay_us

void ultrasonic_init(gpio_num_t triggerPin, gpio_num_t echoPin) {
    gpio_set_direction(triggerPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(echoPin, GPIO_MODE_INPUT);
}

float measure_distance(gpio_num_t triggerPin, gpio_num_t echoPin) {
    // Send trigger pulse
    gpio_set_level(triggerPin, 0);
    esp_rom_delay_us(2); // Replace ets_delay_us with esp_rom_delay_us
    gpio_set_level(triggerPin, 1);
    esp_rom_delay_us(10); // Replace ets_delay_us with esp_rom_delay_us
    gpio_set_level(triggerPin, 0);

    // Wait for echo to go HIGH
    uint32_t timeout = 1000000; // 1 second timeout
    while (gpio_get_level(echoPin) == 0 && timeout--) {
        esp_rom_delay_us(1); // Replace ets_delay_us with esp_rom_delay_us
    }
    uint32_t start_time = esp_timer_get_time();

    // Wait for echo to go LOW
    timeout = 1000000;
    while (gpio_get_level(echoPin) == 1 && timeout--) {
        esp_rom_delay_us(1); // Replace ets_delay_us with esp_rom_delay_us
    }
    uint32_t end_time = esp_timer_get_time();

    uint32_t duration = end_time - start_time;
    float distance = (duration / 2.0f) * 0.0343f;

    if (distance > MAX_DISTANCE) {
        return -1.0f; // out of range
    }

    return distance;
}