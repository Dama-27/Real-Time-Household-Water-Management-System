#include "tds_sensor.h"
#include "esp_adc_cal.h"
#include "driver/adc.h"

static adc_channel_t tds_channel;

void init_tds_sensor(adc_channel_t channel) {
    tds_channel = channel;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(tds_channel, ADC_ATTEN_DB_11); // full-scale 3.3V
}

float read_tds(float temperature) {
    int adc_reading = 0;
    for (int i = 0; i < 64; i++) {
        adc_reading += adc1_get_raw(tds_channel);
    }
    adc_reading /= 64;

    float voltage = (adc_reading / 4095.0) * 3.3;  // in volts
    float tds = (133.42 * voltage * voltage * voltage 
                 - 255.86 * voltage * voltage 
                 + 857.39 * voltage) * 0.5;  // simplified formula

    // Optional: compensate for temperature
    tds = tds / (1.0 + 0.02 * (temperature - 25.0));
    return tds;
}
