#include "firebase.h"
#include "wifi_connect.h"  // assuming you already have a wifi_connect module
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "solenoid_control.h"
#include "temperature_sensor.h"
#include "tds_sensor.h"
#include "relay_control.h"
#include "ultrasonic_control.h"
#include "flow_sensor.h"

#define RELAY3_PIN GPIO_NUM_14 // Relay 3 Pin (motor)
#define TRIG_PIN GPIO_NUM_5    // Ultrasonic Sensor Trig Pin
#define ECHO_PIN GPIO_NUM_18   // Ultrasonic Sensor Echo Pin
// #define relay1Pin GPIO_NUM_26 // Relay 1 Pin (water in from road)
// #define tdsSensorPin GPIO_NUM_36   // TDS Sensor Pin (Analog)

// #define IN_FLOW_VALVE_PIN GPIO_NUM_21 // Inflow Valve Pin
// #define OUT_FLOW_VALVE_PIN GPIO_NUM_22 // Outflow Valve Pin

#define IN_SOLENOID_PIN GPIO_NUM_26 // (relay1, outsolen) ->water in from road
#define OUT_SOLENOID_PIN GPIO_NUM_27

#define MAX_DISTANCE 17
#define MIN_DISTANCE 5



void init_pins(void) {
    // Output config
    gpio_config_t io_conf_out = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << TRIG_PIN) |
                        (1ULL << RELAY3_PIN) |
                        (1ULL << IN_SOLENOID_PIN) |
                        (1ULL << OUT_SOLENOID_PIN)
    };
    gpio_config(&io_conf_out);

    // Input config
    gpio_config_t io_conf_in = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << ECHO_PIN) 
                        // (1ULL << GPIO_INPUT_FLOW_SENSOR) |
                        // (1ULL << GPIO_OUTPUT_FLOW_SENSOR)
    };
    gpio_config(&io_conf_in);

    // Initial states
    relay_init(RELAY3_PIN); // HIGH = OFF depending on relay logic
    solenoid_init(IN_SOLENOID_PIN); // HIGH = OFF for solenoid
    solenoid_init(OUT_SOLENOID_PIN);
    ultrasonic_init(TRIG_PIN, ECHO_PIN);
    init_temperature_sensor();
    init_tds_sensor(ADC1_CHANNEL_6); // e.g. GPIO36 → ADC1_CHANNEL_0 (verify your pin)
    // init_flow_sensor(IN_FLOW_VALVE_PIN); // Flow sensor pin (GPIO_NUM_21)
    // init_flow_sensor(OUT_FLOW_VALVE_PIN); // Flow sensor pin (GPIO_NUM_22)


    // // Set default states (OFF = HIGH for active-low relays/solenoids)
    setRelay(RELAY3_PIN, true);          // Motor OFF
    setSolenoid(IN_SOLENOID_PIN, true);  // Inflow closed
    setSolenoid(OUT_SOLENOID_PIN, false); // Outflow closed

}
bool relay3State = false;


void handleWaterFilling(float distance, gpio_num_t pin) {
    if (distance > MAX_DISTANCE && !relay3State) {
        setRelay(pin, false);  // Turn ON
        relay3State = true;
        printf("Relay turned ON (water filling)\n");
    } else if (distance <= MIN_DISTANCE && relay3State) {
        setRelay(pin, true);  // Turn OFF
        relay3State = false;
        printf("Relay turned OFF (tank full)\n");
    }
}



void app_main(void) {
    wifi_connect(); // Connect to Wi-Fi
    init_pins();
    init_flow_sensors();

    // Simulate the input data (you can update these values dynamically if needed)
    float tankDepth = 21.65;

    while (1) {
        // Send updated data to Firebase

        float dist = measure_distance(TRIG_PIN, ECHO_PIN);
        float water_level = ((tankDepth - dist) / tankDepth) * 100; 
        float temp = read_temperature();
        float tds = read_tds(temp);

        // Determine water quality based on TDS value
        const char* water_quality = (tds > 10) ? "bad" : "good";

        float input_flow = get_flow_rate_lpm(0);
        float output_flow = get_flow_rate_lpm(1);

        float total_in = get_total_litres(0);
        float total_out = get_total_litres(1);

        printf("Distance: %.2f cm\n", dist);
        printf("Water level: %.2f \n", water_level);
        printf("Temp: %.2f \n", temp);
        printf("TDS: %.2f \n", tds);
        printf("Water Quality: %s\n", water_quality);
        printf("Inflow: %.2f \n", input_flow);
        printf("Outflow: %.2f \n", output_flow);
        printf("Total In: %.2f \n", total_in);
        printf("Total Out: %.2f \n", total_out);

        firebase_send_data(input_flow, output_flow, tds, temp, total_in, total_out, water_level, water_quality);
       
        bool mode = firebase_get_button_state("toggleState");
        bool motor = firebase_get_button_state("MotorSwitch");
        

        // Print the button state to the ESP32 console
        ESP_LOGI("MODE", "Button state: %s", mode ? "true" : "false");
        ESP_LOGI("Motor", "Motor On/Off: %s", motor ? "true" : "false");
        
        // if (motor) {
        //     setRelay(RELAY3_PIN, false);
        //     printf("Manually Motor ON\n");
        // } else {
        //     printf("Auto ON\n");
        //     if (mode) {
        //         handleWaterFilling(dist, RELAY3_PIN);
        //         printf("Motor mode\n");
        //     } else {
        //         handleWaterFilling(dist, IN_SOLENOID_PIN);
        //         printf("Road water mode\n");
        //     }
        // }

        if (mode){
            handleWaterFilling(dist, RELAY3_PIN);
        }
        else{
            handleWaterFilling(dist, IN_SOLENOID_PIN);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    }
}
