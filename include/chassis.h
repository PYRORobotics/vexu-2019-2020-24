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

      PIDData pos_pid_data;

    public:
      PYROChassis();
      void set_target_position(double);
      void drive_PID();
      void drive_PID_sync(double);
      static void update_differential_pos(void*)
      {

      }
      PIDController PositionPIDController;

      okapi::ChassisControllerPID driveController;
      okapi::AsyncMotionProfileController MotionController;

      pros::Task t_update_differential_pos;

    };
}

extern okapi::PYROChassis chassis;


#endif
