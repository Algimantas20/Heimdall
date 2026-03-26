#ifndef __H_BMP_280_HPP__
#define __H_BMP_280_HPP__

#include <Adafruit_BMP280.h>
#include <Wire.h>

class H_BMP_280 : public Adafruit_BMP280 {
 private:
  static inline constexpr int kBmpAddr = 0x76;

 public:
  H_BMP_280(TwoWire* theWire = &Wire) : Adafruit_BMP280(theWire) {}

  H_BMP_280(int8_t cspin, SPIClass* theSPI = &SPI)
      : Adafruit_BMP280(cspin, theSPI) {}

  H_BMP_280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin)
      : Adafruit_BMP280(cspin, mosipin, misopin, sckpin) {}

  bool setup();
  void display_data(const float& pressure);
};

#endif  // __H_BMP_280_HPP__