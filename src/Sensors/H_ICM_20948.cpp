#include "Sensors/H_ICM_20948.hpp"

bool H_ICM_20948::setup() {
  if (!init()) {
    Serial.println("ICM init failed");
    return false;
  }

  autoOffsets();

  Serial.println("ICM20948 fully initialized");

  return true;
}

void H_ICM_20948::convert_acc_raw(xyzFloat& data) {
  data.x /= kAccSensitivity;
  data.y /= kAccSensitivity;
  data.z /= kAccSensitivity;
}

void H_ICM_20948::convert_gyr_raw(xyzFloat& data) {
  data.x /= kGyroSensitivity;
  data.y /= kGyroSensitivity;
  data.z /= kGyroSensitivity;
}

H_ICM_20948::Packet H_ICM_20948::read() {
  xyzFloat acc, gyr;

  readSensor();

  getAccRawValues(&acc);
  getGyrRawValues(&gyr);
  convert_acc_raw(acc);
  convert_gyr_raw(gyr);

  return {{acc.x, acc.y, acc.z}, {gyr.x, gyr.y, gyr.z}};
}

void H_ICM_20948::display_data(const Packet& p) const {
  Serial.print("Accel (g): ");
  Serial.print("X=");
  Serial.print(p.acc.x);
  Serial.print(" Y=");
  Serial.print(p.acc.y);
  Serial.print(" Z=");
  Serial.println(p.acc.z);

  Serial.print("Gyro (dps): ");
  Serial.print("X=");
  Serial.print(p.gyr.x);
  Serial.print(" Y=");
  Serial.print(p.gyr.y);
  Serial.print(" Z=");
  Serial.println(p.gyr.z);
}
