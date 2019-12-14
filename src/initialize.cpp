#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		initialize.cpp
// Author:	Brandon Rice
// Created: 13 July 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// Contains the code necessary to initialize the robot such that it is ready for
// either autonomous() or opcontrol().
//
//------------------------------------------------------------------------------


// Function Defintions
// -------------------

static void update_localization(void*)
{
	while(1)
	{
		orientation.setHeading(Arduino.bno->get());
		//std::cout << "Heading: " << orientation.getHeading();// << std::endl;
		//std::cout << "; Heading: " << orientation.getAcceleration(x) << std::endl;
		pros::delay(20);
	}
}

//------------------------------------------------------------------------------
// Function: initialize() :
// ------------------------
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
	//pros::lcd::initialize(); // Uncomment to use PROS's LCD
	Screen screen;
	pros::Task t_update_localization(update_localization,(void*)NULL, TASK_PRIORITY_DEFAULT,
	                          TASK_STACK_DEPTH_DEFAULT, "task");
}


//------------------------------------------------------------------------------
// Function: disabled() :
// ----------------------
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
// Function: competition_initialize() :
// ------------------------------------
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
