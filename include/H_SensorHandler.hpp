#ifndef __H_SENSOR_HANDLER_HPP__
#define __H_SENSOR_HANDLER_HPP__

#include <Wire.h>
#include "Adafruit_BME680.h"
#include "MPU6050.h"

class H_SensorHandler {
 private:
  Adafruit_BME680 bme_;
  MPU6050 mpu_;

  static constexpr int SDA_PIN_ = 21;
  static constexpr int SCL_PIN_ = 22;

  static constexpr int kMPUAddress_ = 0x69;
  static constexpr int kBMEAddress_ = 0x77;

  static constexpr float accel_scale_ = 16384.0f;  // For ±2g
  static constexpr float gyro_scale_ = 131.0f;     // For ±250°/s

 public:
  H_SensorHandler() : mpu_(kMPUAddress_) {}

  ~H_SensorHandler() = default;

  struct __attribute__((__packed__)) Packet {
    unsigned long time;
    float temp;
    float bar;
    float accX, accY, accZ;
    float gyrX, gyrY, gyrZ;
  };

  bool begin();
  bool read(Packet& packet);
  static char* format(char* buffer, size_t size, const Packet& packet);
};

#endif  //!__H_SENSOR_HANDLER_HPP__