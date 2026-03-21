#include "Sensors/H_BMP_280.hpp"
#include <Wire.h>

bool H_BMP_280::setup()
{
    return begin(0x76);
}

void H_BMP_280::display_data(const float& pressure)
{   
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
}