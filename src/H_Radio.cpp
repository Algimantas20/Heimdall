#include "H_Radio.hpp"
#include <cstdarg>
#include <cstdio>

void H_Radio::begin(uint32_t baud) {
  serial_.begin(baud, SERIAL_8N1, RX_PIN_, TX_PIN_);
  delay(100);

  sendATf("AT+ADDRESS=%d", address_);
  delay(100);
  sendATf("AT+NETWORKID=%d", network_id_);
  delay(100);
  sendATf("AT+BAND=%d", band_);
}

void H_Radio::sendAT(const char* msg) {
  serial_.print(msg);
  serial_.print("\r\n");
}

void H_Radio::sendATf(const char* format, ...) {
  char buffer[256];

  va_list args;
  va_start(args, format);
  int len = vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  if (len < 0 || len >= static_cast<int>(sizeof(buffer))) {
    return;
  }

  sendAT(buffer);
}

void H_Radio::sendPacket(const char* c) {
  int len = strlen(c);

  sendATf("AT+SEND=%i,%i,%s", recvr_address_, len, c);
}