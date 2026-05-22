#include "H_Radio.hpp"

#include <cstring>

H_Radio::H_Radio(HardwareSerial& serial) : serial_(serial) {
  serial_mutex_ = xSemaphoreCreateMutex();

  command_queue_ = xQueueCreate(10, sizeof(MotorCommand));
}

void H_Radio::begin(uint32_t baud_rate) {
  serial_.begin(baud_rate);
}

bool H_Radio::sendTelemetry(QueueHandle_t telemetry_queue) {

  TelemetryMessage msg_struct;
  if (xQueueReceive(telemetry_queue, &msg_struct, 0) != pdTRUE) {
    return false;
  }

  const char* message = msg_struct.data;

  return sendPacket(message);
}

bool H_Radio::sendPacket(const char* message) {
  if (!xSemaphoreTake(serial_mutex_, pdMS_TO_TICKS(100))) {
    return false;
  }

  serial_.print("AT+SEND=");
  serial_.println(message);

  xSemaphoreGive(serial_mutex_);

  return true;
}

void H_Radio::processIncomingPackets() {
  if (!xSemaphoreTake(serial_mutex_, pdMS_TO_TICKS(10))) {
    return;
  }

  while (serial_.available()) {
    char c = static_cast<char>(serial_.read());

    if (rx_index_ < (kMaxPacketSize - 1)) {
      rx_buffer_[rx_index_++] = c;
    }

    if (c == '\n') {
      rx_buffer_[rx_index_] = '\0';

      MotorCommand cmd;

      if (parsePacket(rx_buffer_, cmd)) {
        if (xQueueSend(command_queue_, &cmd, 0) != pdTRUE) {
          Serial.println("Command queue full");
        }
      }

      rx_index_ = 0;
    }
  }

  xSemaphoreGive(serial_mutex_);
}

bool H_Radio::popCommand(MotorCommand& cmd) {
  return xQueueReceive(command_queue_, &cmd, 0) == pdTRUE;
}

bool H_Radio::parsePacket(const char* packet, MotorCommand& cmd) {
  if (packet == nullptr) {
    return false;
  }

  if (strncmp(packet, "+RCV=", 5) != 0) {
    return false;
  }

  char temp[kMaxPacketSize];

  strncpy(temp, packet, sizeof(temp) - 1);
  temp[sizeof(temp) - 1] = '\0';

  char* context = nullptr;

  strtok_r(temp, "=", &context);

  char* token = strtok_r(nullptr, ",", &context);
  if (token == nullptr) {
    return false;
  }

  token = strtok_r(nullptr, ",", &context);
  if (token == nullptr) {
    return false;
  }

  const int data_length = atoi(token);

  token = strtok_r(nullptr, ",", &context);
  if (token == nullptr) {
    return false;
  }

  char data[64];

  strncpy(data, token, sizeof(data) - 1);
  data[sizeof(data) - 1] = '\0';

  if (static_cast<int>(strlen(data)) != data_length) {
    return false;
  }

  char* data_context = nullptr;

  char* command_token = strtok_r(data, "+", &data_context);
  char* speed_token = strtok_r(nullptr, "+", &data_context);
  char* duration_token = strtok_r(nullptr, "+", &data_context);

  if (command_token == nullptr || speed_token == nullptr ||
      duration_token == nullptr) {
    return false;
  }

  strncpy(cmd.command, command_token, sizeof(cmd.command) - 1);
  cmd.command[sizeof(cmd.command) - 1] = '\0';

  cmd.speed = atoi(speed_token);
  cmd.duration_ms = atoi(duration_token);

  cmd.valid = true;

  return true;
}