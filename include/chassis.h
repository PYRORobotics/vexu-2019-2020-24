#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "api.h"
#include <functional>
#include "arduino.h"
#include "pid.h"
#include "okapi/api.hpp"

#define WHEELBASE 16.5
#define IDLER_WHEEL_DIAMETER 2.83

namespace okapi
{

  class PYROChassis {
    private:
      const okapi::QLength WHEEL_DIAMETER = 3.95_in;
      const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
      const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);

      const float WHEEL_DIAMETER_INCHES = 3.95;

      okapi::MotorGroup left_motors;
      okapi::MotorGroup right_motors;

      okapi::ADIEncoder encoder_left;
      okapi::ADIEncoder encoder_right;

      PIDData pos_pid_data;

    public:
      PYROChassis();
      void set_target_position(double);
      void drive_PID();
      void drive_PID(okapi::ADIEncoder*, okapi::ADIEncoder*);
      void drive_PID_sync(double, bool = true);
      void turn_PID(okapi::ADIEncoder*, okapi::ADIEncoder*);
      void turn_PID_sync(double, bool = true);
      static void update_differential_pos(void*)
      {
        int i = 0;
        while(1)
        {
          pros::lcd::print(6, "%d", i++);
          pros::delay(20);
        }
      }
      PIDController PositionPIDController;

      okapi::ChassisControllerPID driveController;
      okapi::AsyncMotionProfileController MotionController;

      //pros::Task t_update_differential_pos;

    };
}

extern okapi::PYROChassis chassis;


#endif
