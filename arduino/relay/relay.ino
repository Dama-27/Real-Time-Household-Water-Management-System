// Define the GPIO pin where the relay is connected
const int relayPin = 26;  // Change this to the GPIO pin you are using

// Function to control the relay
void controlRelay(bool state) {
  if (state) {
    digitalWrite(relayPin, HIGH);  // Turn the relay ON
    Serial.println("Relay is ON");
  } else {
    digitalWrite(relayPin, LOW);   // Turn the relay OFF
    Serial.println("Relay is OFF");
  }
}

void setup() {
  // Initialize the relay pin as an output
  pinMode(relayPin, OUTPUT);
  
  // Initialize Serial communication for debugging
  Serial.begin(115200);
  
  // Ensure the relay is off initially
  controlRelay(false);
}

void loop() {
  // Example usage of the controlRelay function
  controlRelay(true);  // Turn the relay ON
  delay(2000);         // Wait for 2 seconds
  
  controlRelay(false); // Turn the relay OFF
  delay(2000);         // Wait for 2 seconds
}