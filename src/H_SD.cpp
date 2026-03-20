#include "H_SD.hpp"
#include <SPI.h>

bool H_SD::init()
{
    delay(200);
    SPI.begin(SCK_PIN_, MISO_PIN_, MOSI_PIN_, CS_PIN_);

    if (!SD.begin(CS_PIN_, SPI, 100000)) 
    {
        Serial.println("SD Card Mount Failed");
        return false;
    }

    if (SD.cardType() == CARD_NONE) 
    {
        Serial.println("No SD card detected");
        return false;
    }

    File file = SD.open("/log.txt", FILE_APPEND);
    if (!file) 
    {
        Serial.println("Failed to open file for writing");
        return false;
    }

    file.println("ESP32 initialized successfully");
    file.close();

    Serial.println("ESP32 initialized successfully");
    return true;
}