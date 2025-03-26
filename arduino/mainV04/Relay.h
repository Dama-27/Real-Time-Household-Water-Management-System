void setRelay(int relayPin, bool relayState) {
  // Set the pin mode to OUTPUT (only done once)
  pinMode(relayPin, OUTPUT);

  // Turn the LED on or off based on the ledState argument
  digitalWrite(relayPin, relayState ? HIGH : LOW);
}
