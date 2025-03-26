#define FLOW_SENSOR_PIN 21

volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned long totalMilliLitres = 0;
unsigned long lastTime = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR pulseCounter() {
  portENTER_CRITICAL_ISR(&mux);
  pulseCount++;
  portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Ensure Serial Monitor initializes properly

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

  Serial.println("Flow Sensor Initialized...");
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastTime >= 1000) { // Execute every 1 second
    lastTime = currentTime;

    portENTER_CRITICAL(&mux);
    int pulseCopy = pulseCount;
    pulseCount = 0; // Reset pulse counter
    portEXIT_CRITICAL(&mux);

    if (pulseCopy > 0) {
      flowRate = (pulseCopy / 7.5); // Convert pulses to L/min
      totalMilliLitres += (flowRate / 60) * 1000; // Convert to mL

      Serial.print("Flow Rate: ");
      Serial.print(flowRate, 2);
      Serial.print(" L/min | Total: ");
      Serial.print(totalMilliLitres);
      Serial.println(" mL");
    } else {
      Serial.println("No flow detected...");
    }
  }
}
