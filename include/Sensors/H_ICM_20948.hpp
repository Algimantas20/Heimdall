#ifndef __H_ICM_20948_HPP__
#define __H_ICM_20948_HPP__

#include <ICM20948_WE.h>

class H_ICM_20948 : public ICM20948_WE
{
private:
    static constexpr uint8_t ICM_ADDR = 0x68;

    static constexpr uint8_t SDA_PIN = 21;
    static constexpr uint8_t SCL_PIN = 22;

    static constexpr float m_AccSensitivity = 16384.0f;  // ±2g
    static constexpr float m_GyroSensitivity = 131.0f;   // ±250 dps

public:
    struct Accel { float x, y, z; };
    struct Gyro  { float x, y, z; };
    struct Mag   { float x, y, z; };

    struct Packet { Accel acc; Gyro gyr; Mag mag; };

    H_ICM_20948(TwoWire &wire = Wire)
        : ICM20948_WE(&wire, ICM_ADDR) {}

    uint8_t sda_pin() { return SDA_PIN; }
    uint8_t scl_pin() { return SCL_PIN; }

    bool setup();
    bool read_data(Packet& p);
    void display_data(const Packet& p) const;

    void convert_acc_raw(xyzFloat& data);
    void convert_gyr_raw(xyzFloat& data);
};

#endif