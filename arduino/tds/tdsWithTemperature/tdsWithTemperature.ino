#define TDS_PIN 34  // ADC pin connected to TDS sensor
#define TEMP_SENSOR_PIN 35 // Example: Analog temperature sensor on GPIO35
#define VREF 3.3    // ESP32 ADC reference voltage
#define SCOUNT 30   // Number of samples for median filter

int analogBuffer[SCOUNT];  // Store ADC values

void setup() {
    Serial.begin(115200);
    pinMode(TDS_PIN, INPUT);
    pinMode(TEMP_SENSOR_PIN, INPUT);
}

// 📌 Function to read temperature from an analog sensor
float getTemperature() {
    int rawValue = analogRead(TEMP_SENSOR_PIN);
    float voltage = rawValue * (VREF / 4095.0); // Convert to voltage
    float temperature = (voltage - 0.5) * 100.0; // Example conversion formula (depends on sensor)
    return temperature;
}

// 📌 Function to get TDS value based on temperature
float getTDS(float temperature) {
    // Read multiple ADC values for stability
    for (int i = 0; i < SCOUNT; i++) {
        analogBuffer[i] = analogRead(TDS_PIN);
    }

    // Apply median filtering
    float medianVoltage = getMedian(analogBuffer, SCOUNT) * (VREF / 4095.0);

    // Temperature compensation
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensatedVoltage = medianVoltage / compensationCoefficient;

    // Convert voltage to TDS value
    float tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage
                    - 255.86 * compensatedVoltage * compensatedVoltage
                    + 857.39 * compensatedVoltage) * 0.5; // Calibration factor

    return tdsValue;
}

void loop() {
    float temperature = getTemperature(); // Get real-time temperature
    float tdsValue = getTDS(temperature); // Get TDS value based on temperature

    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.print(" °C | TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    delay(1000);
}

// 📌 Median filter function
int getMedian(int arr[], int len) {
    int temp[len];
    for (int i = 0; i < len; i++) temp[i] = arr[i];

    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }
    return (len % 2) ? temp[len / 2] : (temp[len / 2] + temp[len / 2 - 1]) / 2;
}
