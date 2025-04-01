#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <stdio.h>

#define TDS_SENSOR_PIN ADC1_CHANNEL_0  // ADC1 Channel 0 (GPIO 36)
#define VREF 1100   // Adjust based on ESP32 ADC calibration (mV)
#define SCOUNT 30   // Number of samples for averaging

// Function to initialize the ADC for TDS sensor
static inline void tds_sensor_init() {
    adc1_config_width(ADC_WIDTH_BIT_12);  // Set ADC width to 12-bit (0-4095)
    adc1_config_channel_atten(TDS_SENSOR_PIN, ADC_ATTEN_DB_11); // Set attenuation for 0-3.3V range
}

// Function to calculate TDS value
static inline float getTdsValue(float temperature) {
    int analogBuffer[SCOUNT];
    float averageVoltage = 0.0;
    float tdsValue = 0.0;

    // Read SCOUNT samples
    for (int i = 0; i < SCOUNT; i++) {
        analogBuffer[i] = adc1_get_raw(TDS_SENSOR_PIN);
        vTaskDelay(pdMS_TO_TICKS(10));  // Small delay to get stable readings
    }

    // Compute average value
    int sum = 0;
    for (int i = 0; i < SCOUNT; i++) {
        sum += analogBuffer[i];
    }
    float rawADC = sum / (float)SCOUNT;

    // Convert ADC raw value to voltage (mV)
    float voltage = (rawADC / 4095.0) * VREF;

    // Temperature compensation
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = voltage / compensationCoefficient;

    // Convert voltage to TDS value (PPM)
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                - 255.86 * compensationVoltage * compensationVoltage
                + 857.39 * compensationVoltage) * 0.5;

    return tdsValue;
}

#endif // TDS_SENSOR_H
