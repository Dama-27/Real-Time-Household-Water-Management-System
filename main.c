#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "relay_control.h"
#include "tds_sensor.h"
#include "temperature.h"
#include "solenoid.h"
#include "ultrasonic.h"

void app_main() {
    gpio_num_t relayPin = GPIO_NUM_5;  // GPIO pin for relay control

    // Initialize all hardware components
    relay_init(relayPin);
    tds_sensor_init();
    initSolenoid();
    initUltrasonic();

    while (1) {
        // Read temperature
        float temp = getTemperature();
        printf("Temperature: %.2f°C\n", temp);

        // Read TDS sensor value (pass temp for compensation)
        float tdsValue = getTdsValue(temp);
        printf("TDS Value: %.2f ppm\n", tdsValue);

        // Read ultrasonic sensor distance
        float distance = getDistance();
        printf("Distance: %.2f cm\n", distance);

        // Control relay based on temperature
        if (temp > 30.0) {  // Example: Turn relay ON if temp > 30°C
            setRelay(relayPin, true);
        } else {
            setRelay(relayPin, false);
        }

        // Control solenoid valve (Example: Open if TDS is too high)
        if (tdsValue > 500) {
            controlSolenoid(true);  // Open valve
        } else {
            controlSolenoid(false); // Close valve
        }

        // Example usage of distance: if distance < 10 cm, do something
        if (distance < 10.0) {
            printf("Object detected nearby! Taking action...\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for 2 seconds before next cycle
    }
}
