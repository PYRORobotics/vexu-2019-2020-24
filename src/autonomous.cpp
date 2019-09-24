#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		autonomous.cpp
// Author:	Brandon Rice
// Created: 13 July 2019
//
// Description:
// ------------
// From PROS:
// * Runs the user autonomous code. This function will be started in its own task
// * with the default priority and stack size whenever the robot is enabled via
// * the Field Management System or the VEX Competition Switch in the autonomous
// * mode. Alternatively, this function may be called in initialize or opcontrol
// * for non-competition testing purposes.
// *
// * If the robot is disabled or communications is lost, the autonomous task
// * will be stopped. Re-enabling the robot will restart the task, not re-start it
// * from where it left off.
//
//------------------------------------------------------------------------------



// Function Defintions
// -------------------


//------------------------------------------------------------------------------
// Function: autonomous() : void
// ----------------------------
// Description:
// 		The function (task) that will be scheduled after initialize(), and after
// 		competition_initialize() if connected to a competition switch. This
//    function will run until whichever happens first: (1) autonomous() exits,
//    (2) the autonomous period ends, or (3) the controllers are disconnected
//    from the competition switch.
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
using namespace okapi;
void autonomous() {
  pros::lcd::print(0, "AUTON");
  okapi::AsyncPosPIDController controller = AsyncControllerFactory::posPID(
    {1, -2, 3, -4},
    ADIGyro('A'),//Arduino.BNO055_Main,
    0.001,
    0.0,
    0.0001);

    controller.setTarget(300.0);
    controller.waitUntilSettled();
    pros::lcd::print(1, "%f", ADIGyro('A').get());
    while(1)
    {
      pros::delay(20);
    }
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
