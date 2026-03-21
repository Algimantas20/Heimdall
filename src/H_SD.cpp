#include "H_SD.hpp"
#include <SPI.h>
#include <ctime>

H_SD::~H_SD()
{
    close_log();
}

bool H_SD::init()
{
    delay(200);
    SPI.begin(SCK_PIN_, MISO_PIN_, MOSI_PIN_, CS_PIN_);

    if (SD.begin(CS_PIN_, SPI, 100000) && SD.cardType() != CARD_NONE) 
    {
        Serial.println("SD Card Mount initialized successfully");
        return true;
    }

    Serial.println("SD Card Mount Failed");
    return false;
}

bool H_SD::init_log(const char* header)
{
    log_file_ = SD.open("/log.csv", FILE_WRITE);

    if (!log_file_) 
    {
        Serial.println("Failed to initialized log file");
        return false;
    }


    log_file_.println(header);
    
    Serial.println("Log file initialized successfully");

    return true;
}

void H_SD::close_log() 
{
    log_file_.flush();
    log_file_.close();
}

bool H_SD::log(const char *data)
{
    if(!log_file_)
    {
        return false;
    }

    log_file_.println(data);
    log_file_.flush();

    return true;
}