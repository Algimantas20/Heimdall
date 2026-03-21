#include <driver/i2c.h>
#include <Arduino.h>
#include <Wire.h>
#include "H_SD.hpp"
#include "Sensors/H_ICM_20948.hpp"
#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_TMP_102.hpp"

#include "H_SensorHandler.hpp"

#define SDA_PIN 21
#define SCL_PIN 22

TwoWire i2c_bus = TwoWire(1);

H_SensorHandler sensors(&i2c_bus);
H_SD sd;


void setup()
{
    Serial.begin(115200);
    delay(500);

    if(!i2c_bus.begin(SDA_PIN, SCL_PIN, 400000))
    {
        Serial.println("I2C init failed");
        while(1);
    }

    bool sd_status = sd.init();
    if(!sd_status){ while(1); }

    bool log_status = sd.init_log("time,temp,bar,accX,accY,accZ,gyrX,gyrY,gyrZ,magX,magY,magZ");
    if(!log_status){ while (1); }

    bool sensor_status = sensors.begin();
    if(!sensor_status){ while(1); }
}

void loop()
{
    static H_SensorHandler::Packet packet;

    sensors.read(packet);
    char *data = sensors.format(packet);

    bool log_status = sd.log(data);
    if(!log_status)
    {
        Serial.println("Failed to log");
    }

    delay(200);
}