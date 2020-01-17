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
PYROChassis::PYROChassis(): PositionPIDController(20, 80, 10, 5.5, 1.5, 0.000005),
                            left_motors({-M_CHASSIS_LF, M_CHASSIS_LM, -M_CHASSIS_LR}),
                            right_motors({-M_CHASSIS_RF, M_CHASSIS_RM, -M_CHASSIS_RR}),
                            encoder_left(ADIEncoder('A', 'B', 0)),
                            encoder_right(ADIEncoder('C', 'D', 1)),
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

//------------------------------------------------------------------------------
// Method: getHighestMotorTemperature() :
// ------------------------------------
// Description:
//     Polls all drive motors and returns the temperature of the hottest motor
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    double maxTemp
//```
//------------------------------------------------------------------------------
double PYROChassis::getHighestMotorTemperature(){
    int motors[] = {M_CHASSIS_LF, M_CHASSIS_LM, M_CHASSIS_LR, M_CHASSIS_RF, M_CHASSIS_RM, M_CHASSIS_RR};
    double maxTemp = pros::Motor(motors[0]).get_temperature();
    for(int i = 1; i < sizeof(motors)/ sizeof(int); i++){
        if(pros::Motor(motors[i]).get_temperature() > maxTemp){
            maxTemp = pros::Motor(motors[i]).get_temperature();
        }
    }
    return maxTemp;
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
