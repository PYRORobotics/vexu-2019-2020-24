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


// okapi::PYROChassis* chassisptr = &chassis;
// double coordinfo[] = {1,2,3};
// asyncDriveStruct ds{
//   chassisptr,
//   coordinfo
// };
//
// pros::Task t_test(drive_to_coordinate_async, (void*)&ds, TASK_PRIORITY_DEFAULT,
//                 TASK_STACK_DEPTH_DEFAULT, "lift auton task");

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
void autonomousSelector(autonNames id)
{
  std::cout << id << std::endl;
  switch (id) {
    case red0:
      lv_obj_set_hidden(Screen::Screen_Plate_R,0);
      auton_red0();
      break;
    case blue0:
      lv_obj_set_hidden(Screen::Screen_Plate_B,0);
      auton_blue0();
      break;
    case red1:
      lv_obj_set_hidden(Screen::Screen_Plate_R,0);
      auton_red1();
      break;
    case blue1:
      lv_obj_set_hidden(Screen::Screen_Plate_B,0);
      auton_blue1();
      break;
    case redSkills0:
      lv_obj_set_hidden(Screen::Screen_Plate_R,0);
      auton_skills0();
      break;
    case blueSkills0:
      lv_obj_set_hidden(Screen::Screen_Plate_B,0);
      auton_skills0();
      break;
    default:
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
  okapi::PYRO_Arduino::send("AUTONOMOUS");

    // Initialize objects for auton
    lift.piston_floor.retract();
    lift.piston_door.retract();

    std::cout << "Auton should be: " << Screen::selectedAuton << std::endl;

    // Run selected auton routine
    autonomousSelector(Screen::selectedAuton);

    // Remove lift task from scheduler
    lifttaskauto.suspend();
    lifttaskauto.remove();

    lv_obj_set_hidden(Screen::Screen_Sponsors,0);
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
  //-----Set Starting Coordinate-----//
  // N/A

  chassis.MotionController.generatePath({Point{0_in,0_in,-17_deg}, Point{12_in,-3.25_in,0_deg}}, "0");

  //-----Deploy-----//
  lift.moveLiftToHeight(11, 50);
  pros::delay(500);
  // Turn on intake
  intake.motors.moveVelocity(150);

  //-----Drive into Cube 1-----//
  chassis.MotionController.setTarget("0", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{15_in,0_in,0_deg}}, "1");
  chassis.MotionController.waitUntilSettled();

  //-----Pick Up Cube 1 (1)-----//
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();
  lift.moveLiftToHeight(26, 100);
  pros::delay(200);

  //-----Intake to U-----//
  intake.motors.tarePosition();
  intake.motors.setBrakeMode(AbstractMotor::brakeMode::hold);
  intake.motors.moveAbsolute(-42, 100);

  //-----Drive Up to 4 Stack-----//
  chassis.MotionController.setTarget("1", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{3_in,0_in,0_deg}}, "ALIGN STACK");
  chassis.MotionController.waitUntilSettled();

  //-----Slowly Align with 4 Stack-----//
  chassis.MotionController.setTarget("ALIGN STACK", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{6_in,0_in,0_deg}}, "3");
  chassis.MotionController.waitUntilSettled();

  //-----Pick Up 4 Stack (5)-----//
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();
  lift.moveLiftToHeight(20, 80);

  //-----Drive Forward to Laterally Align with 3 Stack-----//
  chassis.MotionController.setTarget("3", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{15_in,0_in,0_deg}}, "4");
  chassis.MotionController.waitUntilSettled();

  //-----Turn Facing 3 Stack-----//
  chassis.turn_PID_sync(90.5);

  //-----Drive Up to 3 Stack-----//
  chassis.MotionController.setTarget("4", false);  //Drive forward
  chassis.MotionController.waitUntilSettled();


  //-----Align with 3 Stack and Push Extra Cubes Away-----//
  chassis.MotionController.setTarget("ALIGN STACK", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{6_in,0_in,0_deg}}, "PUSH CUBES AWAY");
  chassis.MotionController.waitUntilSettled();
  chassis.MotionController.setTarget("PUSH CUBES AWAY", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{8_in,0_in,0_deg}}, "6");
  chassis.MotionController.waitUntilSettled();


  //-----Pick Up 3 Stack (8)-----//
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();
  lift.moveLiftToHeight(16, 50);

  // Turn on intake
  intake.motors.moveVelocity(70);


  //-----Drive Up to 2 Stack-----//
  chassis.MotionController.setTarget("6", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{6_in,0_in,0_deg}}, "6.5");
  chassis.MotionController.waitUntilSettled();

  chassis.MotionController.setTarget("6", true);  //Drive backward
  chassis.MotionController.waitUntilSettled();

  //-----Pick Up free cube ()-----//
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  intake.motors.moveVelocity(0);

  lift.moveLiftToHeight(13, 50);

  chassis.MotionController.setTarget("6", false);  //Drive forward
  chassis.MotionController.waitUntilSettled();


  //-----Intake to U-----//
  intake.motors.tarePosition();
  intake.motors.setBrakeMode(AbstractMotor::brakeMode::hold);
  intake.motors.moveAbsolute(-50, 100);

  chassis.MotionController.setTarget("6.5", false);  //Drive forward
  chassis.MotionController.waitUntilSettled();

  //-----Align with 2 Stack and Push Extra Cubes Away-----//
  chassis.MotionController.setTarget("ALIGN STACK", false);  //Drive forward
  chassis.MotionController.waitUntilSettled();
  chassis.MotionController.setTarget("PUSH CUBES AWAY", false);  //Drive forward
  chassis.MotionController.generatePath({Point{0_in,0_in,0_deg}, Point{34_in,-24_in,-38_deg}}, "7");
  chassis.MotionController.waitUntilSettled();

  //-----Pick Up 2 Stack (10)-----//
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();


  //-----Wide Arc Turn into Corner; Pick up cubes alogn the way-----//
  // Turn on intake
  intake.motors.moveVelocity(150);
  chassis.MotionController.setTarget("7", false);  //Drive forward
  chassis.MotionController.waitUntilSettled();

  intake.motors.moveVelocity(0);
  pros::delay(200);
  intake.motors.moveRelative(-190, 100);

  //-----Ram into Corner-----//
  chassis.driveController.driveVector(0.3, 0);
  pros::delay(1500);
  chassis.driveController.stop();

  //-----Back Up Slightly-----// ???
  chassis.driveController.driveVector(-0.05, 0);
  pros::delay(500);
  chassis.driveController.stop();

  //-----Release Stack-----//
  // Place stack on floor
  lift.moveLiftToHeight(0.1, 50);
  pros::delay(1000); // Wait for stack to settle

  // Release piston holding cubes
  lift.piston_floor.extend();
  pros::delay(300);

  lift.moveLiftToHeight(36, 50);
  pros::delay(5000);
  lift.piston_floor.retract();
  lift.moveLiftToHeight(42, 50);
  pros::delay(800);


  // Back away from stack
  chassis.driveController.driveVector(-0.07, 0);
  pros::delay(5000);
  chassis.driveController.stop();
  lift.moveLiftToHeight(6, 50);


  //-----Back Up-----//

  //-----Drive to Alliance Tower Cube-----//
  //-----Drive to Bottom Tower Cube-----//
  //-----Drive to Pyramid-----//
  //-----END RED AUTON-----//

}

void auton_red0BOX()
{
  //-----Set Starting Coordinate-----//

  //-----Deploy-----//
  lift.moveLiftToHeight(11, 50);
  pros::delay(500);
  // Turn on intake
  intake.motors.moveVelocity(150);

  //-----Drive into Cube 1-----//

  //-----Pick Up Cube 1 (1)-----//

  //-----Drive Up to 4 Stack-----//

  //-----Slowly Align with 4 Stack-----//

  //-----Pick Up 4 Stack (5)-----//

  //-----Drive Forward to Laterally Align with 3 Stack-----//

  //-----Turn Facing 3 Stack-----//

  //-----Intake to U-----//

  //-----Drive Up to 3 Stack-----//

  //-----Align with 3 Stack and Push Extra Cubes Away-----//

  //-----Pick Up 3 Stack (8)-----//

  {
  //-----Drive Up to 2 Stack-----//
  //-----Align with 2 Stack and Push Extra Cubes Away-----//
  //-----Pick Up 2 Stack (10)-----//
  }

  //-----Wide Arc Turn into Corner; Pick up cubes alogn the way-----//

  //-----Ram into Corner-----//

  //-----Back Up Slightly-----//

  //-----Release Stack-----//

  //-----Back Up-----//

  //-----Drive to Alliance Tower Cube-----//
  //-----Drive to Bottom Tower Cube-----//
  //-----Drive to Pyramid-----//
}

void auton_red2()
{
    //-----START RED AUTON-----//

    lift.moveLiftToHeight(8, 50);

    // Drive forward to prevent lift colliding with the field wall
    chassis.drive_to_coordinate(6, 0, 0);
    chassis.drive_to_coordinate(2, 0, 0);


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
{};
void auton_blue1()
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
// Function: auton_red1() :
// ------------------------
// Description:
// 		The main code to control the robot using the red1 autonomous routine.
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
void auton_red1()
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

  lift.moveLiftToHeight(11, 50);
  pros::delay(500);
  // Turn on intake
  intake.motors.moveVelocity(150);

  chassis.drive_PID_sync(6);
  chassis.driveController.driveVector(-0.5, 0);
  pros::delay(1000);
  chassis.driveController.stop();



  // START REAL ROUTINE
  chassis.drive_PID_sync( 12.920 ); // Drive forward
  pros::delay(100);

  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  // Tare the lift (requires the lift task to stop clamping)
  lift.tare();
  // pros::delay(500);
  // Done calibrating the lift, start routine

  lifttaskauto.resume();

  chassis.turn_PID_sync( -90.000 ); // Turn
  pros::delay(150);
  chassis.drive_PID_sync( 19.375 ); // Drive forward
  pros::delay(100);



  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  // Simultaniously increase lift height for 4 cube stack
  lift.moveLiftToHeight(34, 80);  // 20, 26, 39
  chassis.drive_seconds(-0.2,0.3);
  pros::delay(600);
  intake.motors.moveVelocity(150);
  pros::delay(150);
  intake.motors.moveRelative(-160, 100);




  chassis.drive_seconds(0.25,0.8);

  lift.piston_floor.extend();
  pros::delay(25);
  lift.piston_floor.retract();

  chassis.drive_PID_sync( -9.375 ); // Drive forward
  pros::delay(100);

  chassis.turn_PID_sync( 90.000 ); // Turn
  pros::delay(150);

  chassis.drive_PID_sync( 12.5 ); // Drive forward
  pros::delay(100);

  chassis.drive_seconds(0.1, 2000);
  pros::delay(100);


}





void auton_skills0OLD()
{
  // Starting Coordinate: (59.220_in, 59.220_in, -90.0_deg) --- now click on the map again to add a second point.

  // Drive forward to prevent lift colliding with the field wall
  chassis.drive_PID_sync(6);
  chassis.drive_PID_sync(-1);


  // // Tare the lift (requires the lift task to stop clamping)
  // lifttaskauto.suspend(); // Not needed?
  // lift.tare(); // Not needed?
  // lifttaskauto.resume(); // Not needed?


  lift.moveLiftToHeight(11, 50);

  // Tare the lift (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.tare();
  lifttaskauto.resume();
  // pros::delay(500);
  // Done calibrating the lift, start routine

  // Turn on intake
  intake.motors.moveVelocity(150);

  // START REAL ROUTINE
  chassis.drive_PID_sync( 5.920 ); // Drive forward
  pros::delay(100);

  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  chassis.turn_PID_sync( -90.000 ); // Turn
  pros::delay(150);
  chassis.drive_PID_sync( 11.375 ); // Drive forward
  pros::delay(100);



  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  // Simultaniously increase lift height for 4 cube stack
  lift.moveLiftToHeight(34, 80);  // 20, 26, 39
  chassis.drive_seconds(-30,0.3);
  chassis.drive_seconds(0,0.1);
  pros::delay(1000);



  chassis.drive_seconds(50,0.8);
  chassis.drive_seconds(0,0.1);

  lift.piston_floor.extend();
  pros::delay(25);
  lift.piston_floor.retract();

  pros::delay(500);


  chassis.drive_PID_sync( -16.5 ); // Drive backward
  pros::delay(150);


  chassis.turn_PID_sync( 92.000 ); // Turn
  pros::delay(150);

  lift.moveLiftToHeight(44, 80);  // 20, 26, 39


  // Go after 4 stack

  intake.motors.moveVelocity(150);
  pros::delay(150);
  intake.motors.moveRelative(-160, 100);

  chassis.drive_PID_sync( 16.25 ); // Drive forward
  pros::delay(150);

  chassis.drive_PID_sync( 3 ); // Drive forward
  pros::delay(150);

  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  lift.moveLiftToHeight(16, 80);  // 20, 26, 39


  chassis.turn_PID_sync(17);

  intake.motors.moveVelocity(150);
  chassis.drive_PID_sync( 15 ); // Drive forward

  chassis.drive_PID_sync( -4 ); // Drive forward
  pros::delay(150);

  // Collect cube (requires the lift task to stop clamping)
  lifttaskauto.suspend();
  lift.collectCube();
  lifttaskauto.resume();

  lift.moveLiftToHeight(35, 150);  // 20, 26, 39
  pros::delay(1200);



  chassis.drive_seconds(50,0.8);
  chassis.drive_seconds(0,0.1);

  pros::delay(150);

  lift.piston_floor.extend();
  pros::delay(30);
  lift.piston_floor.retract();

  pros::delay(500);
  lift.moveLiftToHeight(46, 50);  // 20, 26, 39


  chassis.drive_PID_sync( -16.75 ); // Drive forward
  chassis.turn_PID_sync(-7-45);
  intake.motors.moveRelative(-100, 100);



  chassis.drive_PID_sync( 27.25 ); // Drive forward
  pros::delay(150);

  chassis.drive_seconds(50,0.8);
  chassis.drive_seconds(0,0.1);

  pros::delay(500);

  lift.piston_floor.extend();
  pros::delay(35);
  lift.piston_floor.retract();

  pros::delay(500);


  chassis.drive_PID_sync( -20.125 ); // Drive forward
  lift.moveLiftToHeight(14, 50);  // 20, 26, 39

  // Turn on intake
  intake.motors.moveVelocity(150);

  chassis.turn_PID_sync( 165.000 ); // Turn
  pros::delay(150);
  chassis.drive_PID_sync( 29 ); // Drive forward
  pros::delay(150);

  // // Collect cube (requires the lift task to stop clamping)
  // lifttaskauto.suspend();
  // lift.collectCube();
  // lifttaskauto.resume();




  // chassis.turn_PID_sync( 7 ); // Turn
  // pros::delay(150);

  // Stop the intake and open it to "U" position
  intake.motors.moveRelative(-190, 100);

  chassis.driveController.left(1);
  chassis.driveController.right(-0.9);

  pros::delay(1000);

  chassis.driveController.left(-0.2);
  chassis.driveController.right(0.2);

  pros::delay(150);
  chassis.driveController.driveVector(0, 0);


  // chassis.drive_PID_sync( 23.25 ); // Drive forward
  // pros::delay(150);
  //
  // chassis.drive_seconds(75,1);
  // chassis.drive_seconds(0,0.1);



  // Place stack on floor
  lift.moveLiftToHeight(0.1, 80);
  pros::delay(200); // Wait for stack to settle

  // Release piston holding cubes
  lift.piston_floor.extend();
  pros::delay(100);

  lift.piston_door.extend();
  pros::delay(100);


  //pros::delay(3000);



  chassis.drive_PID_sync( -10 ); // Drive backward


//////////////////////////////




//
//   chassis.drive_PID_sync( 3.661 ); // Drive forward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( -45.500 ); // Turn
//   pros::delay(300);
//
//   // Stop the intake and open it to "U" position
//   intake.motors.moveVelocity(150);
//
//   chassis.drive_PID_sync( 24.979 ); // Drive forward
//   pros::delay(300);
//
//   intake.motors.moveRelative(-80, 80);
//
//
//   chassis.drive_seconds(50,0.5);
//   chassis.drive_seconds(0,0.1);
//
//   lift.piston_floor.extend();
//   pros::delay(40);
//   lift.piston_floor.retract();
// pros::delay(300);
//
//   chassis.drive_PID_sync( -28.189 ); // Drive backward
//   pros::delay(300);
//
//   lift.moveLiftToHeight(10, 50);  // 20, 26, 39
//
//   // Turn on intake
//   intake.motors.moveVelocity(150);
//
//   chassis.turn_PID_sync( 175.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 16.125 ); // Drive forward
//   pros::delay(300);
//
//   // Collect cube (requires the lift task to stop clamping)
//   lifttaskauto.suspend();
//   lift.collectCube();
//   lifttaskauto.resume();
//
//   chassis.turn_PID_sync( 12.000 ); // Turn
//   pros::delay(300);
//
//   // Stop the intake and open it to "U" position
//   intake.motors.moveRelative(-190, 100);
//
//   chassis.drive_PID_sync( 20.319 ); // Drive forward
//   pros::delay(300);
//
//   chassis.drive_seconds(75,1);
//   chassis.drive_seconds(0,0.1);
//
//
//
//   // Place stack on floor
//   lift.moveLiftToHeight(0.1, 50);
//   pros::delay(1000); // Wait for stack to settle
//
//   // Release piston holding cubes
//   lift.piston_floor.extend();
//   pros::delay(300);
//
//   lift.piston_door.extend();
//   pros::delay(300);
//
//
//   //pros::delay(3000);
//
//
//
//   chassis.drive_PID_sync( -20.465 ); // Drive backward
//   pros::delay(300);
//   // Turn on intake
//   intake.motors.moveVelocity(150);
//
//
//   lift.piston_floor.retract();
//   pros::delay(300);
//
//   lift.piston_door.retract();
//   pros::delay(300);
//
//   lift.moveLiftToHeight(11, 50);
//
//   chassis.turn_PID_sync( 135.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( -15.706 ); // Drive backward
//   chassis.drive_seconds(-100,1);
//   chassis.drive_seconds(0,0.1);
//   pros::delay(300);
//
//   chassis.drive_PID_sync( 25.394 ); // Drive forward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( 90.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 25.453 ); // Drive forward
//   pros::delay(300);
//
//   // Collect cube (requires the lift task to stop clamping)
//   lifttaskauto.suspend();
//   lift.collectCube();
//   lifttaskauto.resume();
//
//   chassis.drive_seconds(-30,0.25);
//   chassis.drive_seconds(0,0.1);
//
//   lift.moveLiftToHeight(31, 50);  // 20, 26, 39
//   pros::delay(1000);
//
//   chassis.drive_seconds(50,0.5);
//   chassis.drive_seconds(0,0.1);
//
//   lift.piston_floor.extend();
//   pros::delay(50);
//   lift.piston_floor.retract();
//
//   pros::delay(500);
//
//
//
//   chassis.drive_PID_sync( -7.628 ); // Drive backward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( 90.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 16.531 ); // Drive forward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( -90.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 43.241 ); // Drive forward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( -45.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 36.728 ); // Drive forward
//   pros::delay(300);
//
//   // Collect cube (requires the lift task to stop clamping)
//   lifttaskauto.suspend();
//   lift.collectCube();
//   lifttaskauto.resume();
//
//   chassis.turn_PID_sync( -45.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 9.809 ); // Drive forward
//   pros::delay(300);
//
//   // Collect cube (requires the lift task to stop clamping)
//   lifttaskauto.suspend();
//   lift.collectCube();
//   lifttaskauto.resume();
//
//   chassis.drive_PID_sync( -8.485 ); // Drive backward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( -45.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 25.673 ); // Drive forward
//   pros::delay(300);
//
//   chassis.turn_PID_sync( 50.000 ); // Turn
//   pros::delay(300);
//   chassis.drive_PID_sync( 64.211 ); // Drive forward
//   pros::delay(300);
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
