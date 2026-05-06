#ifndef __H_SENSOR_HANDLER_HPP__
#define __H_SENSOR_HANDLER_HPP__

#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_ICM_20948.hpp"
#include "Sensors/H_TMP_102.hpp"

class H_SensorHandler {
 private:
  H_BMP_280 bmp_;
  H_TMP_102 tmp_;
  H_ICM_20948 icm_;

  static constexpr int SDA_PIN_ = 21;
  static constexpr int SCL_PIN_ = 22;

 public:
  H_SensorHandler(TwoWire* wire = &Wire) : icm_(wire), bmp_(wire), tmp_(wire) {}

  ~H_SensorHandler() = default;

  struct __attribute__((__packed__)) Packet {
    unsigned long time;
    float temp;
    float bar;
    float accX, accY, accZ;
    float gyrX, gyrY, gyrZ;
  };

  bool begin(TwoWire* i2c_bus);
  bool read(Packet& packet);
  static char* format(char* buffer, size_t size, const Packet& packet);
};

#endif  //!__H_SENSOR_HANDLER_HPP__