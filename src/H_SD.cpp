#include "H_SD.hpp"
#include <SPI.h>

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
#if USE_BINARY_LOG
    log_file_ = SD.open("/log.bin", FILE_WRITE);
#else
    log_file_ = SD.open("/log.csv", FILE_WRITE);
#endif

    if (!log_file_) 
    {
        Serial.println("Failed to initialized log file");
        return false;
    }

#if !USE_BINARY_LOG
    log_file_.println(header);
#endif

    Serial.println("Log file initialized successfully");

    return true;
}

void H_SD::close_log() 
{
    if (!log_file_)
    {
        return;
    }
    
    log_file_.flush();
    log_file_.close();
}

bool H_SD::log(const H_SensorHandler::Packet &packet)
{
    if(!log_file_)
    {
        return false;
    }

    size_t written = 0;

#if USE_BINARY_LOG
    written = log_file_.write((const uint8_t*)&packet, sizeof(packet));
#else
    static char buffer[128];
    H_SensorHandler::format(buffer, sizeof(buffer), packet);
    written = log_file_.println(buffer);
#endif

    static uint32_t last_read = 0;
    if(millis() - last_read >= 200)
    {
        log_file_.flush();
        last_read += 200;
    }

    return (written > 0);
}


