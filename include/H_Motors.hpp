#ifndef __H_MOTORS_HPP__
#define __H_MOTORS_HPP__

#include <SparkFun_TB6612.h>
#include "H_Radio.hpp"

class H_Motors {
 private:
  static constexpr int AIN1_PIN_ = 14;
  static constexpr int AIN2_PIN_ = 27;

  static constexpr int BIN1_PIN_ = 33;
  static constexpr int BIN2_PIN_ = 25;

  static constexpr int PWM_PIN_ = 26;
  static constexpr int STBY_PIN_ = 32;

  Motor motor1;
  Motor motor2;

  void moveForward(int speed, int duration);
  void moveBackward(int speed, int duration);
  void turnLeft(int speed, int duration);
  void turnRight(int speed, int duration);

  void stop();

 public:
  H_Motors()
      : motor1(AIN1_PIN_, AIN2_PIN_, PWM_PIN_, 0, STBY_PIN_),
        motor2(BIN1_PIN_, BIN2_PIN_, PWM_PIN_, 0, STBY_PIN_) {}

  void executeCommand(const H_Radio::MotorCommand& cmd);
};

#endif  //!__H_MOTORS_HPP__