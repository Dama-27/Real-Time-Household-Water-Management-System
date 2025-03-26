#define TdsSensorPin 36  // Connect the TDS sensor to GPIO34 (or any ADC pin on ESP32)
#define VREF 3.3         // ESP32 ADC reference voltage
#define ADC_RESOLUTION 4095  // ESP32 ADC is 12-bit (0-4095)

void setup() {
    Serial.begin(115200);
}

void loop() {
    int rawValue = analogRead(TdsSensorPin);  // Read the raw ADC value
    float voltage = (rawValue * VREF) / ADC_RESOLUTION;  // Convert to voltage

    Serial.print("Raw ADC Value: ");
    Serial.print(rawValue);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 3);
    Serial.println(" V");

    delay(1000);  // Delay for stability
}
