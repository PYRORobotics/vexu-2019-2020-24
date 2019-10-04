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


  	// pros::Controller master(pros::E_CONTROLLER_MASTER);
  	// pros::Motor left_mtr(1);
  	// pros::Motor right_mtr(2);
  	// okapi::MotorGroup left_motors({1,-2});
    //   okapi::MotorGroup right_motors({-3,4});


      const okapi::QLength WHEEL_DIAMETER = 3.95_in;
      const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
      const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);
      okapi::ChassisControllerPID driveController1 = ChassisControllerFactory::create(
              {3,4}, {1,-2},
              okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
              okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
              okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
              ratio,
              {WHEEL_DIAMETER, CHASSIS_WIDTH}
      );

      okapi::AsyncMotionProfileController MotionController(AsyncControllerFactory::motionProfile(
        10.0,  // Maximum linear velocity of the Chassis in m/s
        0.5,  // Maximum linear acceleration of the Chassis in m/s/s
        10.0, // Maximum linear jerk of the Chassis in m/s/s/s
        driveController1 // Chassis Controller
      ));




  //
  // pros::lcd::print(0, "AUTON");
  // okapi::AsyncPosPIDController controller = AsyncControllerFactory::posPID(
  //   {1, -2, 3, -4},
  //   ADIGyro('A'),//Arduino.BNO055_Main,
  //   0.001,
  //   0.0,
  //   0.0001);

    //controller.setTarget(300.0);
    //controller.waitUntilSettled();
    //pros::lcd::print(1, "%f", ADIGyro('A').get());
    // while(1)
    // {
    //   pros::delay(20);
    // }

    // Starting Coordinate: (59.175_in, 59.175_in, -90_deg) --- now click on the map again to add a second point.
    // MotionController.generatePath({Point{59.175_in,97.437_in,-90.000_deg}, Point{35.071_in,97.219_in,-90.000_deg}}, "0");
    // MotionController.generatePath({Point{35.071_in,97.219_in,-90.000_deg}, Point{57.221_in,97.219_in,-90.000_deg}}, "1"); 	// Reversed!
    // MotionController.generatePath({Point{57.221_in,97.219_in,-90.000_deg}, Point{12.069_in,35.267_in,-179.600_deg}}, "2");
    // MotionController.generatePath({Point{12.069_in,35.267_in,-179.600_deg}, Point{-52.226_in,18.052_in,50.000_deg}}, "3"); 	// Reversed!
MotionController.reset();
    // Starting Coordinate: (61.564_in, 61.564_in, -90_deg) --- now click on the map again to add a second point.
    MotionController.generatePath({Point{61.500_in,82.264_in,-90.000_deg}, Point{12.285_in,129.394_in,0.000_deg}}, "0");
    MotionController.generatePath({Point{12.285_in,129.394_in,0.000_deg}, Point{23.509_in,34.269_in,0.000_deg}}, "1"); 	// Reversed!
    MotionController.generatePath({Point{23.509_in,34.269_in,0.000_deg}, Point{53.298_in,124.205_in,45.000_deg}}, "2");
    MotionController.generatePath({Point{53.298_in,124.205_in,45.000_deg}, Point{11.853_in,106.045_in,90.000_deg}}, "3"); 	// Reversed!
    MotionController.generatePath({Point{11.853_in,106.045_in,90.000_deg}, Point{59.126_in,82.264_in,90.000_deg}}, "4");



    MotionController.setTarget("0", 0);
    MotionController.waitUntilSettled();
    MotionController.setTarget("1", 1);
    MotionController.waitUntilSettled();
    MotionController.setTarget("2", 0);
    MotionController.waitUntilSettled();
    MotionController.setTarget("3", 1);
    MotionController.waitUntilSettled();
    MotionController.setTarget("4", 0);
    MotionController.waitUntilSettled();
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
