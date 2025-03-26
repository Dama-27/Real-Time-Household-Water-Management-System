const int trigPinUltrasonic = 5;  // GPIO pin connected to the Trig pin of the ultrasonic sensor
const int echoPinUltrasonic = 18; // GPIO pin connected to the Echo pin of the ultrasonic sensor
const int relayPinMotor = 14;  // Change this to the GPIO pin you are using
const int builtInLed = 2;

void setup() {
  Serial.begin(115200); // Start serial communication
  pinMode(trigPinUltrasonic, OUTPUT); // Set the trigPin as an Output
  pinMode(echoPinUltrasonic, INPUT);  // Set the echoPin as an Input
  pinMode(relayPinMotor, OUTPUT);
  pinMode(builtInLed, OUTPUT);

  // Ensure the relay is off initially
  controlRelay(true,relayPinMotor);
}

void loop() {
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance<16){
    controlRelay(true, relayPinMotor);
    digitalWrite(builtInLed, HIGH);
  }
  else{
    controlRelay(false, relayPinMotor);
    digitalWrite(builtInLed, LOW);
  }
  delay(300); // Wait for a second before the next measurement
}

float getDistance() {
  // Clear the trigPin
  digitalWrite(trigPinUltrasonic, LOW);
  delayMicroseconds(2);
  
  // Set the trigPin HIGH for 10 microseconds to send the pulse
  digitalWrite(trigPinUltrasonic, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUltrasonic, LOW);
  
  // Read the echoPin and calculate the duration in microseconds
  long duration = pulseIn(echoPinUltrasonic, HIGH);
  
  // Calculate the distance in centimeters
  float distance = duration * 0.0343 / 2;
  
  return distance;
  // delay(500);
}

void controlRelay(bool state, int relayPin) {
  if (state) {
    digitalWrite(relayPin, HIGH);  // Turn the relay ON
    Serial.println("Relay is ON");
  } else {
    digitalWrite(relayPin, LOW);   // Turn the relay OFF
    Serial.println("Relay is OFF");
  }
}