#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "api.h"
#include <functional>
#include "arduino.h"
#include "pid.h"
#include "okapi/api.hpp"

namespace okapi
{

  class PYROChassis {
    private:
      const okapi::QLength WHEEL_DIAMETER = 3.95_in;
      const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
      const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);

      okapi::MotorGroup left_motors;
      okapi::MotorGroup right_motors;

    public:
      PYROChassis();
      
      PIDController PositionPIDController;

      okapi::ChassisControllerPID driveController;
      okapi::AsyncMotionProfileController MotionController;

    };
}

extern okapi::PYROChassis chassis;


#endif
