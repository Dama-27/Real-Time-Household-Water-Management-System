#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#define TDS_SENSOR_PIN 36  // GPIO pin for the TDS sensor
#define VREF 3.3           // Analog reference voltage (Volt) of the ADC
#define SCOUNT 30          // Number of samples for averaging

// Function to calculate TDS value
float getTdsValue(float temperature = 25.0) {
    int analogBuffer[SCOUNT];
    int analogBufferTemp[SCOUNT];
    int analogBufferIndex = 0;
    float averageVoltage = 0.0;
    float tdsValue = 0.0;

    // Read SCOUNT samples
    for (int i = 0; i < SCOUNT; i++) {
        analogBuffer[i] = analogRead(TDS_SENSOR_PIN);
        delay(10);  // Small delay to get stable readings
    }

    // Copy data to a temporary buffer
    for (int i = 0; i < SCOUNT; i++) {
        analogBufferTemp[i] = analogBuffer[i];
    }

    // Apply median filtering
    int i, j, temp;
    for (j = 0; j < SCOUNT - 1; j++) {
        for (i = 0; i < SCOUNT - j - 1; i++) {
            if (analogBufferTemp[i] > analogBufferTemp[i + 1]) {
                temp = analogBufferTemp[i];
                analogBufferTemp[i] = analogBufferTemp[i + 1];
                analogBufferTemp[i + 1] = temp;
            }
        }
    }

    // Get the median value
    if (SCOUNT % 2 == 0) {
        averageVoltage = ((analogBufferTemp[SCOUNT / 2] + analogBufferTemp[(SCOUNT / 2) - 1]) / 2.0) * (VREF / 4096.0);
    } else {
        averageVoltage = analogBufferTemp[SCOUNT / 2] * (VREF / 4096.0);
    }

    // Temperature compensation
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;

    // Convert voltage to TDS value
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
                - 255.86 * compensationVoltage * compensationVoltage
                + 857.39 * compensationVoltage) * 0.5;

    return tdsValue;
}

#endif // TDS_SENSOR_H
