#define TDS_PIN 34  // ADC pin connected to TDS sensor
#define VREF 3.3    // ESP32 ADC reference voltage (3.3V)
#define SCOUNT 30   // Number of samples for median filter

int analogBuffer[SCOUNT];  // Store ADC values
int analogBufferTemp[SCOUNT];  
int analogBufferIndex = 0;

float temperature = 25.0;  // Adjust based on actual water temperature

void setup() {
    Serial.begin(115200);
    pinMode(TDS_PIN, INPUT);
}

void loop() {
    static unsigned long sampleTime = millis();
    if (millis() - sampleTime > 40) {  // Read ADC every 40ms
        sampleTime = millis();
        analogBuffer[analogBufferIndex] = analogRead(TDS_PIN);
        analogBufferIndex++;
        if (analogBufferIndex == SCOUNT) {
            analogBufferIndex = 0;
        }
    }

    static unsigned long printTime = millis();
    if (millis() - printTime > 800) {  // Print TDS value every 800ms
        printTime = millis();
        
        // Copy buffer and apply median filter
        for (int i = 0; i < SCOUNT; i++) {
            analogBufferTemp[i] = analogBuffer[i];
        }
        float medianVoltage = getMedian(analogBufferTemp, SCOUNT) * (VREF / 4095.0);

        // Temperature compensation
        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
        float compensatedVoltage = medianVoltage / compensationCoefficient;

        // Convert voltage to TDS value
        float tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage
                        - 255.86 * compensatedVoltage * compensatedVoltage
                        + 857.39 * compensatedVoltage) * 0.5;  // Calibration factor 0.5

        Serial.print("TDS Value: ");
        Serial.print(tdsValue, 0);
        Serial.println(" ppm");
    }
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
