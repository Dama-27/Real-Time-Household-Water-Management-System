#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS); // used for temp
DallasTemperature sensors(&oneWire);

// Pin Definitions
const int trigPin = 5;    // Ultrasonic Sensor Trig Pin
const int echoPin = 18;   // Ultrasonic Sensor Echo Pin
const int relay3Pin = 14; // Relay 3 Pin
const int relay1Pin = 26; // Relay 1 Pin
const int tempSensorPin = 4;   // Temperature Sensor Pin
const int tdsSensorPin = 34;   // TDS Sensor Pin (Analog)

const int inflowValvePin = 21; // Inflow Valve Pin
const int outflowValvePin = 22; // Outflow Valve Pin

const int inSolenoidPin = 26;
const int outSolenoidPin = 27;

// Variables
float duration, distance;
float temperature, tdsValue;

bool relay3State = false;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  sensors.begin();

  // Pin Modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(tempSensorPin, INPUT);
  pinMode(tdsSensorPin, INPUT);
  pinMode(inflowValvePin, INPUT);
  pinMode(outflowValvePin, INPUT);

  // Initialize Relays to OFF
  digitalWrite(relay3Pin, LOW);
  digitalWrite(relay1Pin, LOW);
  relay3State = false;

  //initialize solenoids to low
  digitalWrite(inSolenoidPin, LOW);
  digitalWrite(outSolenoidPin, LOW);
  
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2; // Calculate distance in cm

  return distance;
}

void loop() {

  sensors.requestTemperatures(); // Request temperature reading
  float temperature = sensors.getTempCByIndex(0); // Get temperature in Celsius

  float distance = getDistance();
  float tdsValue = analogRead(tdsSensorPin) * (3.3 / 4095.0) * 1000.0;  // Example conversion

  //solenoid
  digitalWrite(inSolenoidPin, HIGH);
  digitalWrite(outSolenoidPin, HIGH);

  int inflow = analogRead(inflowValvePin);
  int outflow = analogRead(outflowValvePin);

  //
  float tankDepth=20;
  float distancePercentage = ((tankDepth-distance)/tankDepth)*100;

  // Display in Serial Monitor
  Serial.println("//////////////////////");
  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
  Serial.print("Distance: "); Serial.print(distancePercentage); Serial.println(" %");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" C");
  Serial.print("TDS Value: "); Serial.print(tdsValue); Serial.println(" ppm");
  Serial.print("Inflow Valve: "); Serial.println(inflow);
  Serial.print("Outflow Valve: "); Serial.println(outflow);

  Serial.println("//////////////////////");
  if (distance > 20 && !relay3State) {
    // If the distance is greater than 20 cm and the relay is off, turn it on
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay3Pin, LOW);
    relay3State = true;

  } else if (distance <= 8 && relay3State) {
    // If the distance is less than or equal to 4 cm and the relay is on, turn it off
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay3Pin, HIGH);
    relay3State = false;
  }

  delay(1000);
}