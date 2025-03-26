#define inVALVE_PIN 26
#define outVALVE_PIN 27

void setup() {
  pinMode(inVALVE_PIN, OUTPUT);
  digitalWrite(inVALVE_PIN, LOW); // Ensure valve is off initially
  pinMode(outVALVE_PIN, OUTPUT);
  digitalWrite(outVALVE_PIN, LOW); // Ensure valve is off initially
}

void loop() {
  digitalWrite(inVALVE_PIN, HIGH); // Turn on the valve
  digitalWrite(outVALVE_PIN, HIGH); // Turn on the valve
  Serial.println("Valve ON");
  delay(5000); // Keep valve open for 5 seconds

  digitalWrite(inVALVE_PIN, LOW); // Turn off the valve
  digitalWrite(outVALVE_PIN, LOW); // Turn off the valve
  Serial.println("Valve OFF");
  delay(5000); // Wait 5 seconds before repeating
}