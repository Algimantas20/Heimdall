#ifndef __H_TMP_102_HPP__
#define  __H_TMP_102_HPP__

#include <Arduino.h>
#include <Wire.h>

class H_TMP_102
{
private:
  TwoWire *wire_;
  static constexpr int TMP_ADDRESS_ = 0x49;

public:
    H_TMP_102(TwoWire* wire = &Wire): wire_(wire) {};

    float readTemperature();
    void display_data(const float& tempreture);
};

#endif