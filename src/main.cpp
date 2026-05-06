#include <Arduino.h>
#include <Wire.h>
#include <driver/i2c.h>

#include "H_Radio.hpp"
#include "H_SD.hpp"
#include "H_SensorHandler.hpp"
#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_ICM_20948.hpp"
#include "Sensors/H_TMP_102.hpp"

#define CSV_HEADER "time,temp,bar,accX,accY,accZ,gyrX,gyrY,gyrZ"

TaskHandle_t SensorTaskHandle = nullptr;
TaskHandle_t RadioRxTaskHandle = nullptr;

TwoWire i2c_bus = TwoWire(1);
HardwareSerial radio_serial(1);

H_SensorHandler sensors(&i2c_bus);

H_Radio radio(radio_serial);
H_SD sd;

void ErrorLedBlink() {
  static bool has_blinked = false;
  static uint32_t last_toggle_time = 0;
  static int toggle_count = 0;
  static bool led_state = HIGH;

  if (has_blinked)
    return;

  uint32_t current_time = millis();

  if (current_time - last_toggle_time >= 100) {
    led_state = !led_state;
    digitalWrite(BUILTIN_LED, led_state);
    last_toggle_time = current_time;
    toggle_count++;

    if (toggle_count >= 20) {
      digitalWrite(BUILTIN_LED, HIGH);
      has_blinked = true;
    }
  }
}

void SensorTask(void* parameter) {
  H_SensorHandler::Packet packet;
  char buffer[128];

  uint32_t last_read = 0;

  while (true) {
    if (millis() - last_read >= 200) {
      last_read = millis();

      sensors.read(packet);

      char* msg = H_SensorHandler::format(buffer, sizeof(buffer), packet);

      if (!sd.log(msg)) {
        ErrorLedBlink();
        Serial.println("Failed to log");
      }

      radio.sendPacket(msg);
    }

    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

void RadioRxTask(void* parameter) {
  while (true) {
    while (radio_serial.available()) {
      char c = radio_serial.read();
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {

  pinMode(2, OUTPUT);

  Serial.begin(115200);
  radio.begin(115200);

  if (!sd.init()) {
    ErrorLedBlink();
    while (true) {}
  }

  if (!sd.init_log(CSV_HEADER)) {
    ErrorLedBlink();
    while (true) {}
  }

  if (!sensors.begin(&i2c_bus)) {
    ErrorLedBlink();
    while (true) {}
  }

  xTaskCreatePinnedToCore(SensorTask, "SensorTask", 8192, nullptr, 2,
                          &SensorTaskHandle, 1);

  xTaskCreatePinnedToCore(RadioRxTask, "RadioRxTask", 4096, nullptr, 3,
                          &RadioRxTaskHandle, 0);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}