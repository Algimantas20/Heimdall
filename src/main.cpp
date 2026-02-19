#include "Config.hpp"
#include <Arduino.h>
#include <Wire.h>          // For I2C communication

#define TMP102_ADDR 0x48   // Default if ADD0 is GND

void get_temp()
{
    Wire.beginTransmission(TMP102_ADDR);
    Wire.write(0x00); 
    Wire.endTransmission();

    Wire.requestFrom(TMP102_ADDR, 2);

    if (Wire.available() != 2) 
    {
        Serial.println("Error reading from TMP102");
        return;
    }
    
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    int16_t temp_raw = ((msb << 8) | lsb) >> 4; // 12-bit value
    float tempC = temp_raw * 0.0625;            // Each LSB = 0.0625°C

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");

}

void setup() 
{
    Serial.begin(115200);
    Wire.begin(21, 22);   // TMP102 SDA = GPIO21, SCL = GPIO22

    Serial.println("Heimdall is running...");
}

void loop() 
{
    get_temp();

    delay(1000);
}
