#include "chassis.h"
#include "../include/pros/adi.h"
#include "arduino.h"

//==================================START FILE==================================
//==============================================================================
// File:		chassis.cpp
// Author:	Brandon Rice
// Created: 7 October 2019
//
// Description:
// ------------
// This file contains the definitions of the PYROChassis class.
//
//------------------------------------------------------------------------------



// File Setup
// ----------

using namespace okapi;


// Global Objects Created in bno055.hpp
// ------------------------------------

PYROChassis chassis;


// Function Defintions
// -------------------



// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROChassis() :
// ----------------------------
// Description:
// 		Constructs a PYROChassis object containing a Position PID Controller and
//    other okapi chassis objects.
//
// Parameters:
//```
//		NONE
//```
// Objects to Initialize:
//```
//		PositionPIDController (PIDController),
//    left_motors (okapi::MotorGroup),
//    right_motors (okapi::MotorGroup),
//    driveController (okapi::ChassisControllerPID),
//    MotionController (okapi::AsyncMotionProfileController)
//    t_update_differential_pos (pros::Task)
//```
//------------------------------------------------------------------------------
PYROChassis::PYROChassis(): PositionPIDController(20, 100, -100, 0.5, 0, 0),
                            left_motors({1,-2}),
                            right_motors({-3,4}),
                            driveController(ChassisControllerFactory::create(
                                    left_motors, right_motors,
                                    okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
                                    okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
                                    okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
                                    ratio,
                                    {WHEEL_DIAMETER, CHASSIS_WIDTH}
                            )),
                            MotionController(AsyncControllerFactory::motionProfile(
                              10.0,  // Maximum linear velocity of the Chassis in m/s
                              0.5,  // Maximum linear acceleration of the Chassis in m/s/s
                              10.0, // Maximum linear jerk of the Chassis in m/s/s/s
                              driveController // Chassis Controller
                            )),
                            t_update_differential_pos(update_differential_pos,NULL)
{
  left_motors.setBrakeMode(AbstractMotor::brakeMode::brake);
  right_motors.setBrakeMode(AbstractMotor::brakeMode::brake);
  pos_pid_data.error = 0;
  pos_pid_data.target_position = 0;
}


//------------------------------------------------------------------------------
// Method: set_target_position(double target_position) :
// ----------------------------
// Description:
// 		Sets the target position (setpoint) used by the Position PID Controller.
//
// Parameters:
//```
//		double target_position
//```
// Returns:
//```
//		void
//```
//------------------------------------------------------------------------------
void PYROChassis::set_target_position(double target_position)
{
  pos_pid_data.target_position = target_position;
}


//------------------------------------------------------------------------------
// Method: drive_PID() :
// ----------------------------
// Description:
// 		Calculates the velocity output from the Position PID and sets the
//    left_motors and right_motors motor groups to run at this output value.
//    NOTE: Place this method in a loop!
//
// Parameters:
//```
//		NONE
//```
// Returns:
//```
//		void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_PID()
{
  int output = (int) PositionPIDController.calculate(pos_pid_data.target_position,
                     left_motors.getPosition(), &pos_pid_data.error);
  left_motors.moveVelocity(output);
  right_motors.moveVelocity(output);
}


//------------------------------------------------------------------------------
// Method: drive_PID() :
// ----------------------------
// Description:
// 		Calculates the velocity output from the Position PID and sets the
//    left_motors and right_motors motor groups to run at this output value.
//    NOTE: Place this method in a loop!
//
// Parameters:
//```
//		NONE
//```
// Returns:
//```
//		void
//```
//------------------------------------------------------------------------------
void PYROChassis::drive_PID_sync(double distance)
{
  set_target_position(distance);
  do
  {
    int output = (int) PositionPIDController.calculate(pos_pid_data.target_position,
                       left_motors.getPosition(), &pos_pid_data.error);
    left_motors.moveVelocity(output);
    right_motors.moveVelocity(output);
  } while(pos_pid_data.error > 5);
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
