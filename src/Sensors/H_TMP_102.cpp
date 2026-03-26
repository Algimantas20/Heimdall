#include "Sensors/H_TMP_102.hpp"

float H_TMP_102::readTemperature() {
  wire_->beginTransmission(TMP_ADDR);
  wire_->write(0x00);
  wire_->endTransmission(false);

  wire_->requestFrom(TMP_ADDR, 2);

  if (wire_->available() < 2) {
    return NAN;
  }

  uint8_t msb = wire_->read();
  uint8_t lsb = wire_->read();

  int16_t rawTemp = (msb << 8) | lsb;
  rawTemp >>= 4;

  if (rawTemp & 0x0800) {
    rawTemp |= 0xF000;
  }

  return rawTemp * 0.0625;
}

void H_TMP_102::display_data(const float& temperature) {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
}