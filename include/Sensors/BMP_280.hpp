#ifndef __BMP_280_HPP__
#define __BMP_280_HPP__

#include <Adafruit_BMP280.h>

class BMP_280 : public Adafruit_BMP280
{
private:
    static constexpr int BMP_280_SDA_PIN_ = 16;
    static constexpr int BMP_280_SCL_PIN_ = 17;

public:
    BMP_280(TwoWire *theWire = &Wire) : Adafruit_BMP280(theWire) {}
    BMP_280(int8_t cspin, SPIClass *theSPI = &SPI) : Adafruit_BMP280(cspin, theSPI) {}
    BMP_280(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin) : Adafruit_BMP280(cspin, mosipin, misopin, sckpin) {}
    
    int get_sda_pin() const { return BMP_280_SDA_PIN_; }
    int get_scl_pin() const { return BMP_280_SCL_PIN_; }

    bool setup();
    static void display_data(const float& pressure, const float& temperature);
};

#endif // __BMP_280_HPP__