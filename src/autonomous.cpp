#include "main.h"
#include "autonomous.h"

//==================================START FILE==================================
//==============================================================================
// File:		autonomous.cpp
// Author:	Brandon Rice
// Created: 13 July 2019
// Last Modified: 30 October 2019
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


// File Setup
// ----------
using namespace okapi;


// Global/Static Objects
// ---------------------
pros::Task lifttaskauto(lift.auton, (void*)NULL, TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "lift auton task");


// Function Defintions
// -------------------

//------------------------------------------------------------------------------
// Function: autonomousSelector(autonID) :
// ---------------------------------------
// Description:
// 		Runs the autonomous routine currently selected with static autonID
//    selectedAutonID.
//
// Parameters:
//```
//		autonID id - the id of the autonomous routine to be run.
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void autonomousSelector(autonID id)
{
  switch (id) {
    case red0:
      auton_red0();
      break;
    case blue0:
      auton_blue0();
      break;
    case skills0:
      auton_skills0();
      break;
  }
}


//------------------------------------------------------------------------------
// Function: autonomous() :
// ------------------------
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
void autonomous()
{
    selectedAutonID=red0; // Use to hardcode auton routine

    // Initialize objects for auton
    lift.piston_floor.retract();
    lift.piston_door.retract();

    // Run selected auton routine
    autonomousSelector(selectedAutonID);

    // Remove lift task from scheduler
    lifttaskauto.suspend();
    lifttaskauto.remove();

    pros::lcd::print(3, "DONE WITH AUTON");
}


//------------------------------------------------------------------------------
// Function: auton_red0() :
// ------------------------
// Description:
// 		The main code to control the robot using the red0 autonomous routine.
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
void auton_red0()
{
    //-----START RED AUTON-----//

    lift.moveLiftToHeight(8, 50);

    // Drive forward to prevent lift colliding with the field wall
    chassis.drive_PID_sync(6);
    chassis.drive_PID_sync(-4);


    // Tare the lift (requires the lift task to stop clamping)
    lifttaskauto.suspend(); // Not needed?
    lift.tare(); // Not needed?
    lifttaskauto.resume(); // Not needed?

    lift.moveLiftToHeight(11, 50);


    // Tare the lift (requires the lift task to stop clamping)
    lifttaskauto.suspend();
    lift.tare();
    lifttaskauto.resume();
    pros::delay(500);
    // Done calibrating the lift, start routine

    // Turn on intake
    intake.motors.moveVelocity(200);

    // Intake first field cube
    chassis.drive_PID_sync(6);
    //pros::delay(500);

    // Collect first field cube (requires the lift task to stop clamping)
    lifttaskauto.suspend();
    lift.collectCube();
    lifttaskauto.resume();

    // Stop the intake and open it to "T" position
    intake.motors.moveVelocity(0);
    pros::delay(200);
    intake.motors.moveRelative(-190, 100);

    // Simultaniously increase lift height for 4 cube stack
    lift.moveLiftToHeight(30, 50);

    // Line up near 4 cube stack
    chassis.drive_PID_sync(22.5);
    pros::delay(300);

    // Slowly drive into stack
    chassis.drive_PID_sync(1.75);
    pros::delay(300);

    // Collect 4 cube stack (requires the lift task to stop clamping)
    lifttaskauto.suspend();
    lift.collectCube();
    lifttaskauto.resume();
    pros::delay(500);

    // Drive backward
    chassis.drive_PID_sync(-17);
    pros::delay(300);

    // Turn to face 7th cube
    chassis.turn_PID_sync(-90);
    pros::delay(300);

    // Resume intake for intaking 7th cube
    intake.motors.moveVelocity(200);

    // Drive into 7th cube
    chassis.drive_PID_sync(15);
    pros::delay(300);
    intake.motors.moveVelocity(80);

    // Collect 7th cube (requires the lift task to stop clamping)
    lifttaskauto.suspend();
    lift.collectCube(100);
    lifttaskauto.resume();

    // Stop the intake and open it to "T" position (to place stack in corner)
    intake.motors.moveVelocity(0);
    pros::delay(300);

    // Turn to face the goal
    chassis.turn_PID_sync(-45);
    pros::delay(300);

    intake.motors.moveRelative(-190, 100);


    // Approch the goal with drive PID
    chassis.drive_PID_sync(18);
    // Drive time-based up and over threshold into corner (encoder wheels not accurate)
    chassis.drive_seconds(50,3.5);
    pros::delay(300);

    chassis.drive_seconds(-50,0.2);
    chassis.drive_seconds(0,0.2);

    //chassis.drive_PID_sync(-1.25);
    //pros::delay(500);


    // Place stack on floor
    lift.moveLiftToHeight(0.1, 50);
    pros::delay(1000); // Wait for stack to settle

    // Release piston holding cubes
    lift.piston_floor.extend();
    pros::delay(300);

    lift.moveLiftToHeight(41, 50);
    pros::delay(3000);

    // Open door
    // lift.piston_door.extend();
    // pros::delay(500);

    // Back away from stack
    chassis.drive_PID_sync(-10);
    lift.moveLiftToHeight(6, 50);
    //-----END RED AUTON-----//
}


//------------------------------------------------------------------------------
// Function: auton_blue0() :
// -------------------------
// Description:
// 		The main code to control the robot using the blue0 autonomous routine.
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
void auton_blue0()
{
  //-----START BLUE AUTON-----//

  lift.moveLiftToHeight(11, 50);

  // Drive forward to prevent lift colliding with the field wall
  chassis.drive_PID_sync(6);
  chassis.drive_PID_sync(-4);


  // Tare the lift (requires the lift task to stop clamping)
  lifttaskauto.suspend(); // Not needed?
  lift.tare(); // Not needed?
  lifttaskauto.resume(); // Not needed?


  lift.moveLiftToHeight(11, 50);



  // Tare the lift (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.tare();
  lifttaskauto.resume();
  pros::delay(500);
  // Done calibrating the lift, start routine

  // Turn on intake
  intake.motors.moveVelocity(150);

  // Intake first field cube
  chassis.drive_PID_sync(6);
  //pros::delay(500);

  // Collect first field cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  // Stop the intake and open it to "T" position
  intake.motors.moveVelocity(0);
  pros::delay(200);
  intake.motors.moveRelative(-190, 100);

  // Simultaniously increase lift height for 4 cube stack
  lift.moveLiftToHeight(30, 50);

  // Line up near 4 cube stack
  chassis.drive_PID_sync(22.5);
  pros::delay(300);

  // Slowly drive into stack
  chassis.drive_PID_sync(1.25);
  pros::delay(300);

  // Collect 4 cube stack (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();
  pros::delay(500);

  // Drive backward
  chassis.drive_PID_sync(-17);
  pros::delay(300);

  // Turn to face 7th cube
  chassis.turn_PID_sync(90);
  pros::delay(300);

  // Resume intake for intaking 7th cube
  intake.motors.moveVelocity(80);

  // Drive into 7th cube
  chassis.drive_PID_sync(15);
  pros::delay(300);

  // Collect 7th cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube(100);
  lifttaskauto.resume();

  // Stop the intake and open it to "T" position (to place stack in corner)
  intake.motors.moveVelocity(0);
  pros::delay(300);

  // Turn to face the goal
  chassis.turn_PID_sync(55);
  pros::delay(300);

  intake.motors.moveRelative(-190, 100);


  // Approch the goal with drive PID
  chassis.drive_PID_sync(18);
  // Drive time-based up and over threshold into corner (encoder wheels not accurate)
  chassis.drive_seconds(50,3.5);
  pros::delay(300);

  chassis.drive_seconds(-50,0.2);
  chassis.drive_seconds(0,0.2);

  //chassis.drive_PID_sync(-1.25);
  //pros::delay(500);


  // Place stack on floor
  lift.moveLiftToHeight(0.1, 50);
  pros::delay(1000); // Wait for stack to settle

  // Release piston holding cubes
  lift.piston_floor.extend();
  pros::delay(300);

  lift.moveLiftToHeight(41, 50);
  pros::delay(3000);

  // Open door
  // lift.piston_door.extend();
  // pros::delay(500);

  // Back away from stack
  chassis.drive_PID_sync(-10);
  lift.moveLiftToHeight(6, 50);

  //-----END BLUE AUTON-----//
}


//------------------------------------------------------------------------------
// Function: auton_skills0() :
// ---------------------------
// Description:
// 		The main code to control the robot using the skills0 autonomous routine.
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
void auton_skills0()
{
  auton_red0();
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
