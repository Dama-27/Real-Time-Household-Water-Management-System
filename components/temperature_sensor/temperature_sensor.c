#include "ds18b20.h"
#include "owb.h"
#include "owb_rmt.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdio.h>

#define SENSOR_GPIO (GPIO_NUM_4)
static const char *TAG = "temperature_sensor";

static owb_rmt_driver_info rmt_driver_info;
static OneWireBus *owb;
static DS18B20_Info *ds18b20_info;

void init_temperature_sensor()
{
    ESP_LOGI(TAG, "Initializing temperature sensor...");

    // Initialize the 1-Wire bus
    owb = owb_rmt_initialize(&rmt_driver_info, SENSOR_GPIO, RMT_CHANNEL_0, RMT_CHANNEL_1);
    owb_use_crc(owb, true);  // Enable CRC checks

    // Allocate memory for the sensor
    ds18b20_info = ds18b20_malloc();
    ds18b20_init_solo(ds18b20_info, owb);  // Initialize assuming only one sensor

    ds18b20_use_crc(ds18b20_info, true);  // Enable CRC for safety
    ds18b20_set_resolution(ds18b20_info, DS18B20_RESOLUTION_12_BIT);
}

float read_temperature()
{
    ds18b20_convert_all(owb);  // Start temperature conversion
    vTaskDelay(pdMS_TO_TICKS(750));  // Wait for conversion (750 ms for 12-bit)

    float temp = 0;
    ds18b20_read_temp(ds18b20_info, &temp);  // Read temperature

    ESP_LOGI(TAG, "Temperature: %.2f°C", temp);
    return temp;
}
