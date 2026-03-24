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
    packet.time = millis();
    packet.bar = bmp_.readPressure();
    packet.temp = tmp_.readTemperature();

    H_ICM_20948::Packet icmPacket = icm_.read();

    packet.accX = icmPacket.acc.x;
    packet.accY = icmPacket.acc.y;
    packet.accZ = icmPacket.acc.z;

    packet.gyrX = icmPacket.gyr.x;
    packet.gyrY = icmPacket.gyr.y;
    packet.gyrZ = icmPacket.gyr.z;

    packet.magX = icmPacket.mag.x;
    packet.magY = icmPacket.mag.y;
    packet.magZ = icmPacket.mag.z;

    return true;
}

char *H_SensorHandler::format(char* buffer, size_t size, const Packet &packet)
{
    snprintf(buffer, size,
        "%lu,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
        packet.time,
        packet.temp,
        packet.bar,
        packet.accX,
        packet.accY,
        packet.accZ,
        packet.gyrX,
        packet.gyrY,
        packet.gyrZ,
        packet.magX,
        packet.magY,
        packet.magZ
    );

    return buffer;
}
