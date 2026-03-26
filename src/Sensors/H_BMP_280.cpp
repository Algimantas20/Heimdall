#include "Sensors/H_BMP_280.hpp"

bool H_BMP_280::setup() {
  return begin(kBmpAddr);
}

void H_BMP_280::display_data(const float& pressure) {
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");
}