#ifndef _CHASSIS_H_
#define _CHASSIS_H_

#include "api.h"
#include <functional>
#include "arduino.h"
#include "pid.h"
#include "okapi/api.hpp"
#include <sstream>
#include <iomanip>


//==================================START FILE==================================
//==============================================================================
// File:    chassis.h
// Author:  Brandon Rice
// Created: 7 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// The header file for chassis.cpp, containing the class declaration of
// PYROChassis.
//
//------------------------------------------------------------------------------

#define WHEELBASE 16.5
#define IDLER_WHEEL_DIAMETER 2.83
extern double ecToDegrees(int ec);


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

      static okapi::ADIEncoder encoder_left;
      static okapi::ADIEncoder encoder_right;

      PIDData pos_pid_data;

    public:
      PYROChassis();
      void set_target_position(double);
      void drive_PID();
      void drive_PID(okapi::ADIEncoder*, okapi::ADIEncoder*);
      void drive_PID_sync(double, bool = true);
      void turn_PID(okapi::ADIEncoder*, okapi::ADIEncoder*);
      void turn_PID_sync(double, bool = true);
      void drive_seconds(int,double);

      void drive_to_coordinate(double, double, double);

      static void update_position(void*)
      {
        encoder_left.reset();
        encoder_right.reset();
        std::cout << "ENC RESET"<< std::endl;

        double enc_last = 0;
        while(1)
        {
          // if(OrientationData::mutex_position.take(100))
          // {
            // ADIEncoder enc_l('A', 'B', 0);
            // ADIEncoder enc_r('C', 'D', 1);
            //OrientationData::setHeading(45);

            float heading = OrientationData::getHeading();
            // float xheading = abs(heading)>2&&abs(heading) < 180?heading:0;

            std::stringstream tmp;
            tmp << std::setprecision(3) << std::fixed << sin(PI/180.0*heading);
            double headingCoeffX = stod(tmp.str());   // new_val = 3.143
            tmp.str(std::string());                  // clear tmp for future use

            tmp << std::setprecision(3) << std::fixed << cos(PI/180.0*heading);
            double headingCoeffY = stod(tmp.str());   // new_val = 3.143
            tmp.str(std::string());                  // clear tmp for future use


            double enc_avg =(encoder_left.get() + encoder_right.get())/2.0;
            double x_new = (ecToDegrees(enc_avg - enc_last))* headingCoeffX + OrientationData::getPosition(x);
            double y_new = (ecToDegrees(enc_avg - enc_last))* headingCoeffY + OrientationData::getPosition(y);



            OrientationData::setPosition(x, x_new);
            OrientationData::setPosition(y, y_new);

            // OrientationData::mutex_position.give();
          // }

          // std::cout << "(x, y, h):" << OrientationData::getPosition(x) << ", " << OrientationData::getPosition(y) << ", " <<OrientationData::getHeading() << std::endl;

          pros::delay(20);
          enc_last = enc_avg;
        }
      }
      PIDController PositionPIDController;

      okapi::ChassisControllerPID driveController;
      okapi::AsyncMotionProfileController MotionController;

      pros::Task t_update_pos;

    };
}

extern okapi::PYROChassis chassis;

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
