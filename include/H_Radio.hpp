#ifndef __H_RADIO_HPP__
#define __H_RADIO_HPP__

#include <Arduino.h>
#include <HardwareSerial.h>
#include <queue>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

class H_Radio {
 public:
  struct MotorCommand {
    bool valid = false;

    char command[16] = {0};

    int speed = 0;
    int duration_ms = 0;
  };

  struct TelemetryMessage {
    char data[128];
  };

  explicit H_Radio(HardwareSerial& serial);

  void begin(uint32_t baud_rate);

  bool sendPacket(const char* message);
  bool sendTelemetry(QueueHandle_t telemetry_queue);

  void processIncomingPackets();
  bool popCommand(MotorCommand& cmd);

 private:
  static constexpr size_t kMaxPacketSize = 128;
  char rx_buffer_[kMaxPacketSize];
  size_t rx_index_ = 0;

  HardwareSerial& serial_;

  SemaphoreHandle_t serial_mutex_;
  QueueHandle_t command_queue_;

  bool parsePacket(const char* packet, MotorCommand& cmd);
};

#endif  //!__H_RADIO_HPP__