#define TRIG_PIN 5
#define ECHO_PIN 18
#define RELAY_1 26
#define RELAY_3 14
#define TEMP_SENSOR 4
#define TDS_SENSOR 34
#define INFLOW_VALVE 21
#define OUTFLOW_VALVE 22

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_3, OUTPUT);
    pinMode(TEMP_SENSOR, INPUT);
    pinMode(TDS_SENSOR, INPUT);
    pinMode(INFLOW_VALVE, INPUT);
    pinMode(OUTFLOW_VALVE, INPUT);
    digitalWrite(RELAY_1, LOW);
    digitalWrite(RELAY_3, LOW);
}

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout of 30ms
    if (duration == 0) return -1; // Return -1 if no valid reading

    return (duration * 0.0343) / 2;
}

void loop() {
    float distance = getDistance();
    float temperature = analogRead(TEMP_SENSOR) * (3.3 / 4095.0) * 100.0;  // Example conversion
    float tdsValue = analogRead(TDS_SENSOR) * (3.3 / 4095.0) * 1000.0;  // Example conversion
    int inflow = analogRead(INFLOW_VALVE);
    int outflow = analogRead(OUTFLOW_VALVE);
    
    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" C");
    Serial.print("TDS Value: "); Serial.print(tdsValue); Serial.println(" ppm");
    Serial.print("Inflow Valve: "); Serial.println(inflow);
    Serial.print("Outflow Valve: "); Serial.println(outflow);

    // if (distance > 16.0) {
    //     digitalWrite(RELAY_1, HIGH);
    //     digitalWrite(RELAY_3, HIGH);
    // } else if (distance < 4.0) {
    //     digitalWrite(RELAY_1, LOW);
    //     digitalWrite(RELAY_3, LOW);
    // }

    if (distance > 4.0 && distance != -1) {
        digitalWrite(RELAY_1, HIGH);
        digitalWrite(RELAY_3, HIGH);
    } else {
        digitalWrite(RELAY_1, LOW);
        digitalWrite(RELAY_3, LOW);
    }

    
    delay(1000);
}