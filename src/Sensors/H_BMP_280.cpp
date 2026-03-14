#include "Sensors/H_BMP_280.hpp"
#include <Wire.h>

bool H_BMP_280::setup()
{
    bool status = begin(0x76);

    Serial.print("BMP-280 begin status: ");
    Serial.println(status ? "OK" : "Failed");

    return status;
}

void H_BMP_280::display_data(const float& pressure)
{   
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
}