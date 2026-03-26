#include "H_SD.hpp"

H_SD::~H_SD() {
  close_log();
}

bool H_SD::init() {
  delay(200);
  SPI.begin(SCK_PIN_, MISO_PIN_, MOSI_PIN_, CS_PIN_);

  if (SD.begin(CS_PIN_, SPI, 100000) && SD.cardType() != CARD_NONE) {
    Serial.println("SD Card Mount initialized successfully");
    return true;
  }

  Serial.println("SD Card Mount Failed");
  return false;
}

bool H_SD::init_log(const char* header) {

  if constexpr (kUseBinaryLog) {
    log_file_ = SD.open("/log.bin", FILE_WRITE);
  } else {
    log_file_ = SD.open("/log.csv", FILE_WRITE);
  }

  if (!log_file_) {
    Serial.println("Failed to initialized log file");
    return false;
  }

  if constexpr (!kUseBinaryLog) {
    log_file_.println(header);
  }

  Serial.println("Log file initialized successfully");

  return true;
}

void H_SD::close_log() {
  if (!log_file_) {
    return;
  }

  log_file_.flush();
  log_file_.close();
}

bool H_SD::log(const H_SensorHandler::Packet& packet) {
  if (!log_file_) {
    return false;
  }

  size_t written = 0;

  if constexpr (kUseBinaryLog) {
    const uint8_t* buffer = reinterpret_cast<const uint8_t*>(&packet);
    written = log_file_.write(buffer, sizeof(packet));
  } else {
    char buffer[128];
    H_SensorHandler::format(buffer, sizeof(buffer), packet);
    written = log_file_.println(buffer);
  }

  static uint32_t last_flush = 0;
  uint32_t now = millis();

  if (now - last_flush >= 200) {
    log_file_.flush();
    last_flush = now;
  }

  if constexpr (kUseBinaryLog) {
    return (written == sizeof(packet));
  } else {
    return (written > 0);
  }
}
