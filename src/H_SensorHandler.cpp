#include "H_SensorHandler.hpp"

bool H_SensorHandler::begin()
{
    if(!icm_.setup()) 
    {
        Serial.println("ICM-20948 failed to initialize");
        return false;
    }

    Serial.println("ICM-20948 initialized");

    if(!bmp_.setup())
    {
        Serial.println("BMP-280 failed to initialize");
        return false;
    }

    Serial.println("BMP-280 initialized");

    return true;
}

bool H_SensorHandler::read(Packet &packet)
{
    packet.pressure = bmp_.readPressure();
    packet.temperature = tmp_.readTemperature();
    packet.imu = icm_.read();

    return true;
}

char* H_SensorHandler::format(const Packet &packet)
{
    static char data[128];
    
    snprintf(data, sizeof(data),
        "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
        millis(),
        packet.temperature,
        packet.pressure,
        packet.imu.acc.x,
        packet.imu.acc.y,
        packet.imu.acc.z,
        packet.imu.gyr.x,
        packet.imu.gyr.y,
        packet.imu.gyr.z,
        packet.imu.mag.x,
        packet.imu.mag.y,
        packet.imu.mag.z
    );

    return data;
}