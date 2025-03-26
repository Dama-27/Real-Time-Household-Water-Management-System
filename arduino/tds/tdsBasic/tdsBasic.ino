#define TDS_SENSOR_PIN 34

void setup() {
  Serial.begin(115200);
}

void loop() {
  int tdsValue = analogRead(TDS_SENSOR_PIN); // Read analog value
  float voltage = tdsValue * (3.3 / 4095.0); // Convert to voltage (ESP32 ADC resolution is 12-bit)
  float tds = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5; // TDS calculation

  Serial.print("TDS Value: ");
  Serial.print(tds);
  Serial.println(" ppm");

  delay(1000); // Wait 1 second
}