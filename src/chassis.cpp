#include "chassis.h"
#include "../include/pros/adi.h"
#include "arduino.h"
#include "init.h"

//==================================START FILE==================================
//==============================================================================
// File:    chassis.cpp
// Author:  Brandon Rice
// Created: 7 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// This file contains the definitions of the PYROChassis class.
//
//------------------------------------------------------------------------------


// File Setup
// ----------
using namespace okapi;


// Global/Static Objects
// ---------------------
PYROChassis chassis;


double ecToDegrees(int ec)
{
  return ec/360.0 * PI * IDLER_WHEEL_DIAMETER;
};

// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROChassis() :
// -----------------------
// Description:
//     Constructs a PYROChassis object containing a Position PID Controller and
//    other okapi chassis objects.
//
// Parameters:
//```
//    NONE
//```
// Objects to Initialize:
//```
//    PositionPIDController (PIDController),
//    left_motors (okapi::MotorGroup),
//    right_motors (okapi::MotorGroup),
//    driveController (okapi::ChassisControllerPID),
//    MotionController (okapi::AsyncMotionProfileController)
//    N/A: t_update_differential_pos (pros::Task)
//```
//------------------------------------------------------------------------------

ADIEncoder PYROChassis::encoder_left('A', 'B', 0);
ADIEncoder PYROChassis::encoder_right('C', 'D', 1);



PYROChassis::PYROChassis(): PositionPIDController(20, 80, 10, 5.5, 1.5, 0.000005),
                            left_motors({-M_CHASSIS_LF, M_CHASSIS_LM, -M_CHASSIS_LR}),
                            right_motors({-M_CHASSIS_RF, M_CHASSIS_RM, -M_CHASSIS_RR}),
                            driveController(ChassisControllerFactory::create(
                                    left_motors, right_motors,
                                    encoder_left, encoder_right,
                                    okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
                                    okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
                                    okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
                                    ratio,
                                    {WHEEL_DIAMETER, CHASSIS_WIDTH}
                            )),
                            MotionController(AsyncControllerFactory::motionProfile(
                              0.5,  // Maximum linear velocity of the Chassis in m/s
                              0.5,  // Maximum linear acceleration of the Chassis in m/s/s
                              10.0, // Maximum linear jerk of the Chassis in m/s/s/s
                              driveController // Chassis Controller
                            ))///,
                            ,t_update_pos(update_position,(void*)NULL, TASK_PRIORITY_DEFAULT,
                                                      TASK_STACK_DEPTH_DEFAULT, "task")
                            ///t_update_differential_pos(update_differential_pos,NULL)
{
  left_motors.setBrakeMode(AbstractMotor::brakeMode::brake);
  right_motors.setBrakeMode(AbstractMotor::brakeMode::brake);
  pos_pid_data.error = 0;
  pos_pid_data.target_position = 0;
}


//------------------------------------------------------------------------------
// Method: set_target_position(double) :
// -------------------------------------
// Description:
//     Sets the target position (setpoint) used by the Position PID Controller.
//
// Parameters:
//```
//    double target_position
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::set_target_position(double target_position)
{
  pos_pid_data.target_position = target_position;
}


//------------------------------------------------------------------------------
// Method: drive_PID() :
// ---------------------
// Description:
//     Calculates the velocity output from the Position PID and sets the
//    left_motors and right_motors motor groups to run at this output value.
//    NOTE: Place this method in a loop or use drive_PID_sync() instead!
//
// Parameters:
//```
//    NONE
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_PID()
{
  int output = (int) PositionPIDController.calculate(pos_pid_data.target_position,
                     left_motors.getPosition() * IDLER_WHEEL_DIAMETER * PI / 3600,
                     &pos_pid_data.error);
  left_motors.moveVelocity(output);
  right_motors.moveVelocity(output);
}


//------------------------------------------------------------------------------
// Method: drive_PID(okapi::ADIEncoder*, okapi::ADIEncoder*) :
// -----------------------------------------------------------
// Description:
//     Overloaded function to make use of external adi quadrature encoders.
//
// Parameters:
//```
//    NONE
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_PID(okapi::ADIEncoder* left, okapi::ADIEncoder* right)
{
  int output_l = (int) PositionPIDController.calculate(pos_pid_data.target_position,
                     (left->get()) * IDLER_WHEEL_DIAMETER * PI / 360,
                     &pos_pid_data.error);

  int output_r = - (int) PositionPIDController.calculate(pos_pid_data.target_position,
                    (right->get()) * IDLER_WHEEL_DIAMETER * PI / 360,
                    &pos_pid_data.error);

  pros::lcd::print(6, "%f", pos_pid_data.error);
  pros::lcd::print(7, "%d", output_l);
  left_motors.moveVelocity(output_l);
  right_motors.moveVelocity(output_r);
}


//------------------------------------------------------------------------------
// Method: drive_PID_sync(double, bool) :
// --------------------------------------
// Description:
//     The actual PID "contoller" using drive_PID's calculaitons and outputs.
//
// Parameters:
//```
//    double distance - Distance to drive in inches (signed)
//    bool useIdler - Set to 0 to use the internal motor encoders rather than
//    external adi quadrature encoders.
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_PID_sync(double distance, bool useIdler)
{
  set_target_position(distance);
  encoder_left.reset();
  encoder_right.reset();
  do
  {
    if(!useIdler)
      drive_PID();
    else drive_PID(&encoder_left, &encoder_right);
    pros::delay(20);
  } while(pos_pid_data.error > 0.5);
  left_motors.moveVelocity(0);
  right_motors.moveVelocity(0);

}


//------------------------------------------------------------------------------
// Method: turn_PID(okapi::ADIEncoder*, okapi::ADIEncoder*) :
// ----------------------------------------------------------
// Description:
//     Calculates the velocity output from the Position PID and sets the
//    left_motors and right_motors motor groups to run at this output value.
//    One output is negated, such that the robot will point turn.
//    NOTE: Place this method in a loop or use turn_PID_sync() instead!
//
// Parameters:
//```
//    NONE
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::turn_PID(okapi::ADIEncoder* left, okapi::ADIEncoder* right)
{
  int output_l = (int) PositionPIDController.calculate(pos_pid_data.target_position,
                     (left->get()) * IDLER_WHEEL_DIAMETER * PI / 360,
                     &pos_pid_data.error);

  int output_r =  (int) PositionPIDController.calculate(pos_pid_data.target_position,
                    (-right->get()) * IDLER_WHEEL_DIAMETER * PI / 360,
                    &pos_pid_data.error);

  pros::lcd::print(6, "%f", pos_pid_data.error);
  pros::lcd::print(7, "%d", output_l);
  left_motors.moveVelocity(output_l);
  right_motors.moveVelocity(output_r);
}


//------------------------------------------------------------------------------
// Method: turn_PID_sync(double, bool) :
// -------------------------------------
// Description:
//     The actual PID "contoller" using turn_PID's calculaitons and outputs.
//
// Parameters:
//```
//    double degrees - Angle to drive in degrees (signed, positive Clockwise)
//    bool useIdler - Set to 0 to use the internal motor encoders rather than
//    external adi quadrature encoders.
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::turn_PID_sync(double degrees, bool useIdler)
{

  set_target_position(degrees / 360 * PI * WHEELBASE * 92/95);
  encoder_left.reset();
  encoder_right.reset();
  pos_pid_data.error = 0;
  pros::delay(100);
  do
  {
    if(!useIdler)
      drive_PID();
    else turn_PID(&encoder_left, &encoder_right);
    pros::delay(20);
  } while(pos_pid_data.error > 0.5);
  left_motors.moveVelocity(0);
  right_motors.moveVelocity(0);

}


//------------------------------------------------------------------------------
// Method: drive_seconds(int, double) :
// ------------------------------------
// Description:
//     Runs the motors at int speed (RPM) for sec seconds.
//
// Parameters:
//```
//    int speed - RPM to drive motors at (signed, positive forward)
//    double sec - seconds to drive at this speed (sync.)
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_seconds(int speed, double sec)
{
  left_motors.moveVelocity(speed);
  right_motors.moveVelocity(-speed);
  pros::delay(sec*1000);
}


void PYROChassis::drive_to_coordinate(double x1, double y1, double h1)
{

  double cv = 0, pv = 2; float setpoint = 0;

  PIDControllerRemake q(setpoint, pv, cv, 2, 1, 1, 0, 100, 333);

  double x0, y0, h0;
  x0 = OrientationData::getPosition(x);
  y0 = OrientationData::getPosition(y);
  h0 = OrientationData::getHeading();

  double distance0 = sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );

  q.setSetpoint(distance0);

  float multiplierl = 0.01;
  float multiplierr = 0.01;

  float maxSpeed = 10;
  float kp = 1;

  do
  {
    //std::cout << "sp : " << q.setpoint << std::endl;



    double pidl = cv;
    double pidr = cv;

    double heading_delta;

    if((y1-y0) == 0)
    {
      if(x1-x0 >= 0)
      heading_delta = 90 - h0;
      else
      heading_delta = -90 + h0;
    }
    else
    {
      heading_delta = 180.0/PI * ( atan( (x1-x0)/(y1-y0) ) ) - h0;
    }

    double speedl;
    double speedr;


    if(heading_delta >= 0)
    {
      if(y1-y0 >= 0)
      speedl = 1 * pidl;
      else
      speedl = -1 * pidl;
      speedr = 0.7* cos( PI/180.0 * heading_delta ) * pidr;
    }
    else
    {
      if(y1-y0 >= 0)
      speedr = 1 * pidr;
      else
      speedr = -1 * pidr;
      speedl = 0.7* cos( PI/180.0 * heading_delta ) * pidl;
    }

    speedl /= 100;
    speedr /= 100;

    if(pv < (distance0 * 1 / 4))
    {
      //limit speed
      speedl *= multiplierl;
      speedr *= multiplierr;
      multiplierl += 0.05;
      multiplierr += 0.05;
    }

    // left_motors.moveVelocity(pidl);
    // right_motors.moveVelocity(pidr);

    if(speedl > 0.6)
      speedl = 0.6;
    if(speedr > 0.6)
      speedr = 0.6;

    chassis.driveController.left(speedl);
    chassis.driveController.right(-speedr);

    x0 = OrientationData::getPosition(x);
    y0 = OrientationData::getPosition(y);
    h0 = OrientationData::getHeading();

    pros::delay(20);

    pv = distance0 - sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );
    // std::cout << " pv = " << pv;
    std::cout << "Distance: " << pv << "; Speed l = " << speedl << ", r = " << speedr << " HEADING: " << h0 << ", Delta: " << heading_delta << std::endl;

  } while(fabs(distance0 - pv) > 1);

  pros::delay(100);

  if(h1-OrientationData::getHeading() >= 0)
  {
    while(h1-OrientationData::getHeading() > 0)
    {
      chassis.driveController.left(0.025);
      chassis.driveController.right(0.025);
      pros::delay(20);
    }
  }
  else
  {
    while(h1-OrientationData::getHeading() < 0)
    {
      chassis.driveController.left(-0.025);
      chassis.driveController.right(-0.025);
      pros::delay(20);
    }
  }


  // while(fabs(h1 - h0) > 0.5)
  // {
  //   kp = 1;
  //   double speed;
  //
  //   speed = kp * (h1 - h0);
  //
  //   if(speed > 0.5) speed = 0.5;
  //   else if(speed < -0.5) speed = -0.5;
  //
  //   if((h1 - h0) > 0)
  //   {
  //     chassis.driveController.left(speed);
  //     chassis.driveController.right(speed);
  //   }
  //   else
  //   {
  //     chassis.driveController.left(-speed);
  //     chassis.driveController.right(-speed);
  //   }
  //   pros::delay(20);
  // }

  chassis.driveController.left(0);
  chassis.driveController.right(0);


  // turn_PID_sync(h1 - h0);

}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
