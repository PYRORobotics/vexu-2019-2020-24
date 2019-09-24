#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		initialize.cpp
// Author:	Brandon Rice
// Created: 13 July 2019
//
// Description:
// ------------
// Contains the code necessary to initialize the robot such that it is ready for
// either autonomous() or opcontrol().
//
//------------------------------------------------------------------------------



// Function Defintions
// -------------------
void init_chassis()
{
	const okapi::QLength WHEEL_DIAMETER = 3.95_in;
	const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
	const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);

	//auto controller = okapi::AsyncControllerFactory::posPID({-1, 2}, BNO055_Main, 0.001, 0.0, 0.0001);
	okapi::ChassisControllerPID driveController = ChassisControllerFactory::create(
		 {1,-2}, {3,-4},
		 okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
		 okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
		 okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
		 ratio,
		 {WHEEL_DIAMETER, CHASSIS_WIDTH}
	 );



	  okapi::AsyncMotionProfileController MotionController = AsyncControllerFactory::motionProfile(
	    10.0,  // Maximum linear velocity of the Chassis in m/s
	    0.5,  // Maximum linear acceleration of the Chassis in m/s/s
	    10.0, // Maximum linear jerk of the Chassis in m/s/s/s
	    driveController // Chassis Controller
	  );

}


//------------------------------------------------------------------------------
// Function: initialize() : void
// ----------------------------
// Description:
//   From PROS:
// * Runs initialization code. This occurs as soon as the program is started.
// *
// * All other competition modes are blocked by initialize; it is recommended
// * to keep execution time for this mode under a few seconds.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void initialize()
{
	pros::lcd::initialize();
}


//------------------------------------------------------------------------------
// Function: disabled() : void
// ----------------------------
// Description:
//   From PROS:
// * Runs while the robot is in the disabled state of Field Management System or
// * the VEX Competition Switch, following either autonomous or opcontrol. When
// * the robot is enabled, this task will exit.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void disabled() {}


//------------------------------------------------------------------------------
// Function: competition_initialize() : void
// ----------------------------
// Description:
//   From PROS:
// * Runs after initialize(), and before autonomous when connected to the Field
// * Management System or the VEX Competition Switch. This is intended for
// * competition-specific initialization routines, such as an autonomous selector
// * on the LCD.
// *
// * This task will exit when the robot is enabled and autonomous or opcontrol
// * starts.
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void competition_initialize() {}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
