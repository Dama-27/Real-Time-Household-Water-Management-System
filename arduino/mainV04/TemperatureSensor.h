#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#define DS18B20_PIN 4 // GPIO pin for the DS18B20 sensor

// Function to send a single bit to the sensor
void writeBit(bool bitValue) {
    pinMode(DS18B20_PIN, OUTPUT);
    digitalWrite(DS18B20_PIN, LOW);
    delayMicroseconds(2);

    if (bitValue) {
        digitalWrite(DS18B20_PIN, HIGH);
    }
    delayMicroseconds(60);
    digitalWrite(DS18B20_PIN, HIGH);
}

// Function to write a byte to the DS18B20 sensor
void writeByte(byte data) {
    for (int i = 0; i < 8; i++) {
        writeBit(data & (1 << i));
        delayMicroseconds(60);
    }
}

// Function to read a single bit from the sensor
bool readBit() {
    pinMode(DS18B20_PIN, OUTPUT);
    digitalWrite(DS18B20_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(DS18B20_PIN, HIGH);
    delayMicroseconds(10);

    pinMode(DS18B20_PIN, INPUT);
    bool bitValue = digitalRead(DS18B20_PIN);
    delayMicroseconds(50);

    return bitValue;
}

// Function to read a byte from the DS18B20 sensor
byte readByte() {
    byte value = 0;
    for (int i = 0; i < 8; i++) {
        if (readBit()) {
            value |= (1 << i);
        }
        delayMicroseconds(60);
    }
    return value;
}

// Function to reset the DS18B20 and check if it's present
bool resetSensor() {
    pinMode(DS18B20_PIN, OUTPUT);
    digitalWrite(DS18B20_PIN, LOW);
    delayMicroseconds(480);
    digitalWrite(DS18B20_PIN, HIGH);
    delayMicroseconds(70);

    pinMode(DS18B20_PIN, INPUT);
    bool presence = !digitalRead(DS18B20_PIN);
    delayMicroseconds(410);

    return presence;
}

// Function to get temperature reading from DS18B20
float getTemperature() {
    if (!resetSensor()) {
        return -127.0; // Return error value if sensor not found
    }

    writeByte(0xCC); // Skip ROM command
    writeByte(0x44); // Convert T command
    delay(750); // Wait for conversion (750ms max)

    if (!resetSensor()) {
        return -127.0; // Return error value if sensor not found
    }

    writeByte(0xCC); // Skip ROM command
    writeByte(0xBE); // Read Scratchpad command

    byte lsb = readByte();
    byte msb = readByte();

    int16_t rawTemperature = (msb << 8) | lsb;

    // Convert raw value to Celsius
    float temperature = rawTemperature / 16.0;

    return temperature;
}

#endif // TEMPERATURE_SENSOR_H
