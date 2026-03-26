#ifndef __H_ICM_20948_HPP__
#define __H_ICM_20948_HPP__

#include <ICM20948_WE.h>

class H_ICM_20948 : public ICM20948_WE {
 private:
  static inline constexpr float kAccSensitivity = 16384.0f;  // ±2g
  static inline constexpr float kGyroSensitivity = 131.0f;   // ±250 dps

  static inline constexpr int kIcmAddr = 0x68;

 public:
  struct Accel {
    float x, y, z;
  };

  struct Gyro {
    float x, y, z;
  };

  struct Mag {
    float x, y, z;
  };

  struct Packet {
    Accel acc;
    Gyro gyr;
    Mag mag;
  };

  H_ICM_20948(TwoWire* wire = &Wire) : ICM20948_WE(wire, kIcmAddr) {}

  bool setup();
  Packet read();

  void display_data(const Packet& p) const;

  void convert_acc_raw(xyzFloat& data);
  void convert_gyr_raw(xyzFloat& data);
};

#endif