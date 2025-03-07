#include "driver/gpio.h"
#include "esp_timer.h"

// Pin Definitions
#define TRIG_PIN      GPIO_NUM_5    // Ultrasonic Trig
#define ECHO_PIN      GPIO_NUM_18   // Ultrasonic Echo
#define RELAY1_PIN    GPIO_NUM_26   // Relay 1
#define RELAY3_PIN    GPIO_NUM_14   // Relay 3

void setup() {
    // Initialize GPIOs
    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(RELAY1_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(RELAY3_PIN, GPIO_MODE_OUTPUT);

    // Start relays in OFF state (assuming active-low)
    gpio_set_level(RELAY1_PIN, 1);
    gpio_set_level(RELAY3_PIN, 1);
}

float get_distance() {
    gpio_set_level(TRIG_PIN, 0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    uint32_t duration = pulseIn(ECHO_PIN, 1, 10000); // 10ms timeout
    return (duration * 0.0343) / 2; // cm
}

void loop() {
    float distance = get_distance();

    if (distance > 20) {
        gpio_set_level(RELAY1_PIN, 0); // Both relays ON
        gpio_set_level(RELAY3_PIN, 0);
    } else if (distance <= 4) {
        gpio_set_level(RELAY1_PIN, 1); // Both relays OFF
        gpio_set_level(RELAY3_PIN, 1);
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
}
