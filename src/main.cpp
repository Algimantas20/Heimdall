#include <Arduino.h>
#include <Wire.h>
#include <driver/i2c.h>

#include "H_SD.hpp"
#include "H_SensorHandler.hpp"
#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_ICM_20948.hpp"
#include "Sensors/H_TMP_102.hpp"

#define SDA_PIN 21
#define SCL_PIN 22
#define CSV_HEADER "time,temp,bar,accX,accY,accZ,gyrX,gyrY,gyrZ,magX,magY,magZ"

TwoWire i2c_bus = TwoWire(1);

H_SensorHandler sensors(&i2c_bus);
H_SD sd;

void setup() {
  Serial.begin(115200);
  delay(500);

  if (!i2c_bus.begin(SDA_PIN, SCL_PIN, 400000)) {
    Serial.println("I2C init failed");
    while (true) {}
  }

  bool sd_status = sd.init();
  if (!sd_status) {
    while (true) {}
  }

  bool log_status = sd.init_log(CSV_HEADER);
  if (!log_status) {
    while (true) {}
  }

  bool sensor_status = sensors.begin();
  if (!sensor_status) {
    while (true) {}
  }
}

void loop() {
  static H_SensorHandler::Packet packet;

  static uint32_t last_read = 0;
  if (millis() - last_read >= 200) {
    last_read = millis();

    sensors.read(packet);

    bool log_status = sd.log(packet);
    if (!log_status) {
      Serial.println("Failed to log");
    }
  }
}