#include <driver/i2c.h>
#include <Arduino.h>
#include <Wire.h>
#include "Config.hpp"
#include "Sensors/H_ICM_20948.hpp"

TwoWire icm_bus = TwoWire(1);
H_ICM_20948 icm(icm_bus);

void scan_i2c(TwoWire &wire)
{
    Serial.println("Scanning I2C bus...");

    for(uint8_t addr = 1; addr < 127; addr++)
    {
        wire.beginTransmission(addr);
        if(wire.endTransmission() == 0)
        {
            Serial.print("Found device at 0x");
            Serial.println(addr, HEX);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    icm_bus.begin(icm.sda_pin(), icm.scl_pin(), 100000);
    delay(100);

    scan_i2c(icm_bus);

    if(!icm.setup())
    {
        Serial.println("IMU setup failed");
        while(1);
    }

    delay(500);

}

void loop()
{
    H_ICM_20948::Packet packet;

    if(icm.read_data(packet) && packet.acc.x != 0)
    {
        icm.display_data(packet);
    }

    delay(200);
}