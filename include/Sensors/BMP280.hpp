#ifndef __BMP280_HPP__
#define __BMP280_HPP__

#include <Adafruit_BMP280.h>

class BMP280 : public Adafruit_BMP280
{
private:
    static constexpr int BMP280_SDA_PIN_ = 16;
    static constexpr int BMP280_SCL_PIN_ = 17;

public:
    BMP280(TwoWire *theWire = &Wire) : Adafruit_BMP280(theWire) {}
    BMP280(int8_t cspin, SPIClass *theSPI = &SPI) : Adafruit_BMP280(cspin, theSPI) {}
    BMP280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin) : Adafruit_BMP280(cspin, mosipin, misopin, sckpin) {}

    bool setup();
    static void display_data(float pressure, float temperature);
};

#endif // __BMP280_HPP__