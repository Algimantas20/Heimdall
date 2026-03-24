#ifndef __H_SD__
#define __H_SD__

#include "H_SensorHandler.hpp"
#include <SD.h>

#define USE_BINARY_LOG 0

class H_SD
{
private:
    static constexpr int SCK_PIN_   = 18;
    static constexpr int MISO_PIN_  = 19;
    static constexpr int MOSI_PIN_  = 23;
    static constexpr int CS_PIN_    = 5;

    File log_file_;

public:
    H_SD() = default;
    ~H_SD();

    bool init();

    bool init_log(const char *data);
    void close_log();

    bool log(const H_SensorHandler::Packet &packet);
};

#endif //!__H_SD__
