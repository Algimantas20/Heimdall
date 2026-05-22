#include "H_SensorHandler.hpp"

bool H_SensorHandler::begin() {

  if (!Wire.begin(SDA_PIN_, SCL_PIN_, 400000)) {
    Serial.println("I2C init failed");
    return false;
  }

  if (!bme_.begin(kBMEAddress_)) {
    Serial.println("BME680 init failed");
    return false;
  }

  mpu_.initialize();
  if (!mpu_.testConnection()) {
    Serial.println("MPU6050 connection failed");
    return false;
  }

  return true;
}

bool H_SensorHandler::read(Packet& packet) {
  packet.time = millis();
  packet.temp = bme_.readTemperature();
  packet.bar = bme_.readPressure() / 100.0f;  // Convert to hPa
  packet.accX = static_cast<float>(mpu_.getAccelerationX()) / accel_scale_;
  packet.accY = static_cast<float>(mpu_.getAccelerationY()) / accel_scale_;
  packet.accZ = static_cast<float>(mpu_.getAccelerationZ()) / accel_scale_;
  packet.gyrX = static_cast<float>(mpu_.getRotationX()) / gyro_scale_;
  packet.gyrY = static_cast<float>(mpu_.getRotationY()) / gyro_scale_;
  packet.gyrZ = static_cast<float>(mpu_.getRotationZ()) / gyro_scale_;

  return true;
}

char* H_SensorHandler::format(char* buffer, size_t size, const Packet& packet) {
  const int len =
      snprintf(buffer, size, "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f, %.2f",
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