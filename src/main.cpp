#include <driver/i2c.h>
#include <Arduino.h>
#include <Wire.h>
#include "Config.hpp"
#include "Sensors/H_ICM_20948.hpp"
#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_TMP_102.hpp"

#define SDA_PIN 21
#define SCL_PIN 22

TwoWire i2c_bus = TwoWire(1);

H_ICM_20948 icm(&i2c_bus);
H_BMP_280   bmp(&i2c_bus);
H_TMP_102   tmp(&i2c_bus);

H_ICM_20948::Packet icm_packet;

void sensor_init()
{
    if(!icm.setup())
    {
        Serial.println("ICM setup failed");
        while(1);
    }

    if(!bmp.setup())
    {
        Serial.println("BMP setup failed");
        while(1);
    }
}

void setup()
{
    Serial.begin(115200);

    if(!i2c_bus.begin(SDA_PIN, SCL_PIN, 100000))
    {
        Serial.println("I2C init failed");
        while(1) {}
    }

    sensor_init();
    
    delay(200);
}

void loop()
{
    if(icm.read_data(icm_packet) && icm_packet.acc.x != 0)
    {
        icm.display_data(icm_packet);
    }

    float pressure = bmp.readPressure();
    bmp.display_data(pressure);

    float tempreture = tmp.readTemperature();
    if (tempreture != NAN)
    {
        tmp.display_data(tempreture);
    }

    Serial.println();

    delay(200);
}