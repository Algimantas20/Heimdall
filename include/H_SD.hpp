#ifndef __H_SD__
#define __H_SD__

#include <SD.h>

class H_SD
{
private:
    static constexpr int SCK_PIN_   = 18;
    static constexpr int MISO_PIN_  = 19;
    static constexpr int MOSI_PIN_  = 23;
    static constexpr int CS_PIN_    = 5;

public:
    H_SD() = default;
    ~H_SD() = default;

    bool init();
};

#endif //!__H_SD__
