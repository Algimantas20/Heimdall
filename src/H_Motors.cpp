#include "H_Motors.hpp"

void H_Motors::moveForward(int speed, int duration) {
  motor1.drive(speed, duration);
  motor2.drive(speed, duration);
}

void H_Motors::moveBackward(int speed, int duration) {
  motor1.drive(-speed, duration);
  motor2.drive(-speed, duration);
}

void H_Motors::stop() {
  motor1.brake();
  motor2.brake();
};

void H_Motors::turnRight(int speed, int duration) {
  motor1.drive(speed, duration);
  motor2.drive(-speed, duration);
}

void H_Motors::turnLeft(int speed, int duration) {
  motor1.drive(-speed, duration);
  motor2.drive(speed, duration);
}

void H_Motors::executeCommand(const H_Radio::MotorCommand& cmd) {
  if (!cmd.valid) {
    return;
  }

  if (strcmp(cmd.command, "M_Forward") == 0) {
    moveForward(cmd.speed, cmd.duration_ms);
  } else if (strcmp(cmd.command, "M_Backward") == 0) {
    moveBackward(cmd.speed, cmd.duration_ms);
  } else if (strcmp(cmd.command, "M_Left") == 0) {
    turnLeft(cmd.speed, cmd.duration_ms);
  } else if (strcmp(cmd.command, "M_Right") == 0) {
    turnRight(cmd.speed, cmd.duration_ms);
  } else {
    stop();
  }
}