#ifndef ULTRASONIC_CONTROL_H
#define ULTRASONIC_CONTROL_H

#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define MAX_DISTANCE 400 // Maximum measurable distance in cm

// Function to initialize the ultrasonic sensor pins (Trigger and Echo)
static inline void ultrasonic_init(gpio_num_t triggerPin, gpio_num_t echoPin) {
    gpio_set_direction(triggerPin, GPIO_MODE_OUTPUT);   // Trigger Pin as output
    gpio_set_direction(echoPin, GPIO_MODE_INPUT);       // Echo Pin as input
}

// Function to measure the distance using the ultrasonic sensor
static inline float measure_distance(gpio_num_t triggerPin, gpio_num_t echoPin) {
    // Send a 10us pulse to trigger the sensor
    gpio_set_level(triggerPin, 0);
    ets_delay_us(2);
    gpio_set_level(triggerPin, 1);
    ets_delay_us(10); // Trigger pulse width is 10us
    gpio_set_level(triggerPin, 0);

    // Wait for Echo to go high
    while (gpio_get_level(echoPin) == 0); // Wait for rising edge of Echo
    uint32_t start_time = esp_timer_get_time(); // Start the timer

    // Wait for Echo to go low
    while (gpio_get_level(echoPin) == 1); // Wait for falling edge of Echo
    uint32_t end_time = esp_timer_get_time(); // End the timer

    // Calculate distance: distance = (time / 2) * speed_of_sound
    // Speed of sound = 34300 cm/s, and we divide by 2 to get the one-way distance
    uint32_t duration = end_time - start_time;
    float distance = (duration / 2.0) * 0.0343; // Convert to cm

    // If the measured distance exceeds MAX_DISTANCE, return a value indicating out of range
    if (distance > MAX_DISTANCE) {
        return -1.0f; // Out of range
    }

    return distance; // Return distance in cm
}

#endif // ULTRASONIC_CONTROL_H
