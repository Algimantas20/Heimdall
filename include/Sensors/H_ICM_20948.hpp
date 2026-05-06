#ifndef __H_ICM_20948_HPP__
#define __H_ICM_20948_HPP__

#include <ICM20948_WE.h>

class H_ICM_20948 : public ICM20948_WE {
 private:
  static constexpr float kAccSensitivity = 16384.0f;  // ±2g
  static constexpr float kGyroSensitivity = 131.0f;   // ±250 dps

  static constexpr int kIcmAddr = 0x68;

 public:
  struct Accel {
    float x, y, z;
  };

  struct Gyro {
    float x, y, z;
  };

  struct Packet {
    Accel acc;
    Gyro gyr;
  };

  H_ICM_20948(TwoWire* wire = &Wire) : ICM20948_WE(wire, kIcmAddr) {}

  bool setup();
  Packet read();

  void display_data(const Packet& p) const;

  void convert_acc_raw(xyzFloat& data);
  void convert_gyr_raw(xyzFloat& data);
};

#endif