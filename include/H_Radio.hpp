#ifndef __H_RADIO_HPP__
#define __H_RADIO_HPP__

#include <HardwareSerial.h>

class H_Radio {
 private:
  static constexpr int address_ = 1;
  static constexpr int recvr_address_ = 2;
  static constexpr int network_id_ = 17;
  static constexpr int band_ = 868500000;

  static constexpr int TX_PIN_ = 17;
  static constexpr int RX_PIN_ = 16;

  HardwareSerial serial_;

 public:
  H_Radio(HardwareSerial& serial) : serial_(serial) {};
  ~H_Radio() = default;

  int get_receiver() const { return recvr_address_; }

  void begin(uint32_t baud);

  void sendAT(const char* msg);
  void sendATf(const char* format, ...);

  void sendPacket(const char* msg);
};

#endif  //!__H_RADIO_HPP__