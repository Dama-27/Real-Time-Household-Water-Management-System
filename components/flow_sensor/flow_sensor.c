// flow_sensor.c
#include "flow_sensor.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define GPIO_INPUT_FLOW_SENSOR GPIO_NUM_21
#define GPIO_OUTPUT_FLOW_SENSOR GPIO_NUM_22

static volatile uint32_t pulse_count_input = 0;
static volatile uint32_t pulse_count_output = 0;

static float total_litres_input = 0.0;
static float total_litres_output = 0.0;

static bool isr_service_installed = false;

static void IRAM_ATTR flow_input_isr_handler(void* arg) {
    pulse_count_input++;
}

static void IRAM_ATTR flow_output_isr_handler(void* arg) {
    pulse_count_output++;
}

void init_flow_sensors() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pin_bit_mask = (1ULL << GPIO_INPUT_FLOW_SENSOR) | (1ULL << GPIO_OUTPUT_FLOW_SENSOR)
    };
    gpio_config(&io_conf);

    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }

    gpio_isr_handler_add(GPIO_INPUT_FLOW_SENSOR, flow_input_isr_handler, NULL);
    gpio_isr_handler_add(GPIO_OUTPUT_FLOW_SENSOR, flow_output_isr_handler, NULL);
}

float get_flow_rate_lpm(flow_sensor_type_t sensor_type) {
    uint32_t start_count, end_count;
    float* total_litres_ptr;
    volatile uint32_t* pulse_count_ptr;

    if (sensor_type == FLOW_INPUT) {
        pulse_count_input = 0;
        pulse_count_ptr = &pulse_count_input;
        total_litres_ptr = &total_litres_input;
    } else {
        pulse_count_output = 0;
        pulse_count_ptr = &pulse_count_output;
        total_litres_ptr = &total_litres_output;
    }

    start_count = *pulse_count_ptr;
    uint64_t start_time = esp_timer_get_time();

    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second

    end_count = *pulse_count_ptr;
    uint64_t end_time = esp_timer_get_time();

    uint32_t pulses = end_count - start_count;
    float duration = (end_time - start_time) / 1000000.0;

    float frequency = pulses / duration;
    float flow_rate = frequency / 7.5;
    float litres_passed = pulses / 450.0;

    *total_litres_ptr += litres_passed;

    return flow_rate;
}

float get_total_litres(flow_sensor_type_t sensor_type) {
    if (sensor_type == FLOW_INPUT) {
        return total_litres_input;
    } else {
        return total_litres_output;
    }
}
