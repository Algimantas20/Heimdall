#include <Arduino.h>
#include <Wire.h>
#include "Config.hpp"
#include "Sensors/BMP280.hpp"

BMP280 bmp;

void setup()
{
    Serial.begin(Config::UPLOAD_RATE);

    if (!bmp.setup()) 
    {
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        while (1);
    }

    Serial.println("Sensors ready!");
}

void loop()
{
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();
    
    BMP280::display_data(pressure, temperature);

    delay(1000);
}
