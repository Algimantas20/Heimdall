#include <Arduino.h>
#include <Wire.h>
#include <driver/i2c.h>
#include <queue>

#include "H_Motors.hpp"
#include "H_Radio.hpp"
#include "H_SD.hpp"
#include "H_SensorHandler.hpp"

QueueHandle_t telemetry_queue;

TaskHandle_t SensorTaskHandle = nullptr;
TaskHandle_t RadioRxTaskHandle = nullptr;

HardwareSerial radio_serial(1);

H_SensorHandler sensors;

H_Radio radio(radio_serial);
H_Motors motors;
H_SD sd;

void SensorTask(void* parameter) {
  H_SensorHandler::Packet packet;
  char buffer[128];

  uint32_t last_read = 0;

  while (true) {

    last_read = millis();

    H_Radio::TelemetryMessage msg;

    sensors.read(packet);

    strncpy(msg.data, H_SensorHandler::format(buffer, sizeof(buffer), packet),
            sizeof(msg.data) - 1);
    msg.data[sizeof(msg.data) - 1] = '\0';

    if (!sd.log(msg.data)) {
      Serial.println("Failed to log");
    }

    Serial.println(msg.data);

    xQueueSend(telemetry_queue, &msg, 0);

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void RadioRxTask(void* parameter) {
  while (true) {
    H_Radio::MotorCommand cmd;

    bool status = radio.sendTelemetry(telemetry_queue);
    //Serial.println(status ? "Telemetry sent" : "Failed to send telemetry");

    radio.processIncomingPackets();
    if (radio.popCommand(cmd)) {
      motors.executeCommand(cmd);
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);
  radio.begin(115200);

  telemetry_queue = xQueueCreate(10, sizeof(H_Radio::TelemetryMessage));

  if (!sd.init()) {
    radio.sendPacket("SD init failed");
    while (true) {}
  }

  if (!sd.init_log()) {
    radio.sendPacket("Failed to initialize log file");
    while (true) {}
  }

  if (!sensors.begin()) {
    radio.sendPacket("Failed to initialize sensors");
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