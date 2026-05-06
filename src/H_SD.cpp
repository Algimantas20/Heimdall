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

#if USE_BINARY_LOG
  log_file_ = SD.open("/log.bin", FILE_WRITE);
#else
  log_file_ = SD.open("/log.csv", FILE_WRITE);
#endif

  if (!log_file_) {
    Serial.println("Failed to initialized log file");
    return false;
  }
#if !USE_BINARY_LOG
  log_file_.println(header);
#endif

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

bool H_SD::log(const char* buffer) {

  if (!log_file_) {
    return false;
  }

  size_t written = log_file_.println(buffer);

  static uint32_t last_flush = 0;
  uint32_t now = millis();

  if (now - last_flush >= 200) {
    log_file_.flush();
    last_flush = now;
  }

  return (written > 0);
}