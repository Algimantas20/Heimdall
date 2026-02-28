#include "Sensors/H_ICM_20948.hpp"

bool H_ICM_20948::setup()
{
    if(!init())
    {
        Serial.println("ICM init failed");
        return false;
    }

    initMagnetometer();

    setAccRange(ICM20948_ACC_RANGE_2G);
    setGyrRange(ICM20948_GYRO_RANGE_250);
    setMagOpMode(AK09916_CONT_MODE_100HZ);

    autoOffsets();

    return true;
}

void H_ICM_20948::convert_acc_raw(xyzFloat& data)
{
    data.x /= m_AccSensitivity;
    data.y /= m_AccSensitivity;
    data.z /= m_AccSensitivity;
}

void H_ICM_20948::convert_gyr_raw(xyzFloat& data)
{
    data.x /= m_GyroSensitivity;
    data.y /= m_GyroSensitivity;
    data.z /= m_GyroSensitivity;
}

bool H_ICM_20948::read_data(Packet& p)
{
    xyzFloat acc, gyr, mag;
    
    readSensor();

    getAccRawValues(&acc);
    getGyrRawValues(&gyr);
    getMagValues(&mag);

    convert_acc_raw(acc);
    convert_gyr_raw(gyr);

    p.acc = { acc.x, acc.y, acc.z };
    p.gyr = { gyr.x, gyr.y, gyr.z };
    p.mag = { mag.x, mag.y, mag.z };

    return true;
}

void H_ICM_20948::display_data(const Packet& p) const
{
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

    Serial.print("Mag (µT): ");
    Serial.print("X=");
    Serial.print(p.mag.x);
    Serial.print(" Y=");
    Serial.print(p.mag.y);
    Serial.print(" Z=");
    Serial.println(p.mag.z);

    Serial.println();
}
