#ifndef __H_SENSOR_HANDLER_HPP__
#define __H_SENSOR_HANDLER_HPP__

#include "Sensors/H_BMP_280.hpp"
#include "Sensors/H_TMP_102.hpp"
#include "Sensors/H_ICM_20948.hpp"

class H_SensorHandler
{
private:
    H_BMP_280 bmp_;
    H_TMP_102 tmp_;
    H_ICM_20948 icm_;

public:

    H_SensorHandler(TwoWire *wire = &Wire): bmp_(wire), tmp_(wire), icm_(wire) {}
    ~H_SensorHandler() = default;

    struct Packet
    {
        float temperature;
        float pressure;
        H_ICM_20948::Packet imu;
    };

    bool begin();
    bool read(Packet &packet);

    char *format(const Packet &packet);
};

#endif //!__H_SENSOR_HANDLER_HPP__