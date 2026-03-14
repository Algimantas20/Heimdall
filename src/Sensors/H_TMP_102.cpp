#include "Sensors/H_TMP_102.hpp"

float H_TMP_102::readTemperature()
{
    wire_->beginTransmission(TMP102_ADDRESS);
    wire_->write(0x00);
    wire_->endTransmission(false);

    wire_->requestFrom(TMP102_ADDRESS, 2);
    if(wire_->available() < 2) return NAN;

    uint8_t msb = wire_->read();
    uint8_t lsb = wire_->read();

    int16_t rawTemp = ((msb << 8) | lsb) >> 4;
    float celsius = rawTemp * 0.0625;

    return celsius;
}

void H_TMP_102::display_data(const float& temperature)
{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
}