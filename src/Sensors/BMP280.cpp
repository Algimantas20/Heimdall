#include "Sensors/BMP280.hpp"
#include <Wire.h>

bool BMP280::setup()
{
    Wire.begin(BMP280_SDA_PIN_, BMP280_SCL_PIN_);

    bool status = begin(0x76);

    Serial.print("BMP280 begin status: ");
    Serial.println(status ? "OK" : "Failed");

    return status;
}

void BMP280::display_data(float pressure, float temperature)
{   
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.println();
}