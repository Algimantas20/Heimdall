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

 public:
  H_SensorHandler(TwoWire* wire = &Wire) : bmp_(wire), tmp_(wire), icm_(wire) {}

  ~H_SensorHandler() = default;

  struct __attribute__((packed)) Packet {
    uint32_t time;
    float temp;
    float bar;
    float accX, accY, accZ;
    float gyrX, gyrY, gyrZ;
    float magX, magY, magZ;
  };

  bool begin();
  bool read(Packet& packet);

  static char* format(char* buffer, size_t size, const Packet& packet);
};

#endif  //!__H_SENSOR_HANDLER_HPP__