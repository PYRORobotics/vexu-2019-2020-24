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

  pros::Task lifttaskauto(lift.auton, (void*)NULL, TASK_PRIORITY_DEFAULT,
                  TASK_STACK_DEPTH_DEFAULT, "My Task");

    // chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{12_in,0_in,0_deg}}, "0");
    //
    // chassis.MotionController.setTarget("0", 0);
    //
    // chassis.MotionController.waitUntilSettled();

    lift.piston_floor.retract();
    lift.piston_door.retract();

    // RED
    chassis.drive_PID_sync(2);
    lifttaskauto.suspend();
    lift.tare();
    lifttaskauto.resume();
    lift.moveLiftToHeight(8, 50);
    lifttaskauto.suspend();
    lift.tare();
    lifttaskauto.resume();


    pros::delay(500);

    intake.motors.moveVelocity(150);

    chassis.drive_PID_sync(6);
    pros::delay(500);
    lifttaskauto.suspend();
    lift.collectCube();
    lifttaskauto.resume();


    intake.motors.moveVelocity(0);
    pros::delay(500);
    intake.motors.moveRelative(-190, 100);
    lift.moveLiftToHeight(30, 50);

    chassis.drive_PID_sync(22.5);
    pros::delay(500);
    chassis.drive_PID_sync(1);
    pros::delay(500);
    lifttaskauto.suspend();
    lift.collectCube();
    lifttaskauto.resume();
    pros::delay(500);


    chassis.drive_PID_sync(-17);
    pros::delay(1000);

    chassis.turn_PID_sync(-90);
    pros::delay(1000);

    intake.motors.moveVelocity(80);


    chassis.drive_PID_sync(15);
    pros::delay(500);
    lifttaskauto.suspend();
    lift.collectCube();
    lifttaskauto.resume();

    // chassis.drive_PID_sync(12);
    // pros::delay(1000);

    intake.motors.moveVelocity(0);
    pros::delay(500);
    intake.motors.moveRelative(-190, 100);


    chassis.turn_PID_sync(-45);
    pros::delay(2000);

    chassis.drive_PID_sync(18);
    chassis.drive_seconds(50,1.5);
    pros::delay(500);
    lift.moveLiftToHeight(0.1, 50);
    pros::delay(1500);
    lift.piston_floor.extend();
    pros::delay(500);
    lift.piston_door.extend();
    pros::delay(500);
    chassis.drive_PID_sync(-10);


    pros::lcd::print(3, "DONE WITH AUTON");
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
