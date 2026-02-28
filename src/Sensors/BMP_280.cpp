#include "Sensors/BMP_280.hpp"
#include <Wire.h>

bool BMP_280::setup()
{
    bool status = begin(0x76);

    Serial.print("BMP-280 begin status: ");
    Serial.println(status ? "OK" : "Failed");

    return status;
}

void BMP_280::display_data(const float& pressure, const float& temperature)
{   
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    Serial.println();
}