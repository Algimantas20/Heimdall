#include "H_SensorHandler.hpp"

bool H_SensorHandler::begin(TwoWire* i2c_bus) {

  if (!i2c_bus->begin(SDA_PIN_, SCL_PIN_, 400000)) {
    Serial.println("I2C init failed");
    while (true) {}
  }

  if (!bmp_.setup()) {
    Serial.println("BMP-280 failed to initialize");
    return false;
  }

  Serial.println("BMP-280 initialized");

  if (!icm_.setup()) {
    Serial.println("ICM-20948 failed to initialize");
    return false;
  }

  Serial.println("ICM-20948 initialized");

  return true;
}

bool H_SensorHandler::read(Packet& packet) {
  packet.time = millis();
  packet.bar = bmp_.readPressure();
  packet.temp = tmp_.readTemperature();

  H_ICM_20948::Packet icmPacket = icm_.read();

  packet.accX = icmPacket.acc.x;
  packet.accY = icmPacket.acc.y;
  packet.accZ = icmPacket.acc.z;

  packet.gyrX = icmPacket.gyr.x;
  packet.gyrY = icmPacket.gyr.y;
  packet.gyrZ = icmPacket.gyr.z;

  return true;
}

char* H_SensorHandler::format(char* buffer, size_t size, const Packet& packet) {
  const int len =
      snprintf(buffer, size, "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
               packet.time, packet.temp, packet.bar, packet.accX, packet.accY,
               packet.accZ, packet.gyrX, packet.gyrY, packet.gyrZ);

  if (len < 0) {
    if (size > 0) {
      buffer[0] = '\0';
    }
    return buffer;
  }

  if (static_cast<size_t>(len) >= size) {
    buffer[size - 1] = '\0';
  }

  return buffer;
}