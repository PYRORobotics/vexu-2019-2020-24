#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		opcontrol.cpp
// Author:	Brandon Rice
// Created: 13 July 2019
//
// Description:
// ------------
// From PROS:
// * Runs the operator control code. This function will be started in its own task
// * with the default priority and stack size whenever the robot is enabled via
// * the Field Management System or the VEX Competition Switch in the operator
// * control mode.
// *
// * If no competition control is connected, this function will run immediately
// * following initialize().
// *
// * If the robot is disabled or communications is lost, the
// * operator control task will be stopped. Re-enabling the robot will restart the
// * task, not resume it from where it left off.
//
//------------------------------------------------------------------------------



// Function Defintions
// -------------------


// FIXME - Add Header?
void startAllTasks()	//FIXME
{
}


//------------------------------------------------------------------------------
// Function: opcontrol() : void
// ----------------------------
// Description:
// 		The function (task) that will be scheduled after initialize(), and after
// 		competition_initialize() and autonomous() if connected to a competition
// 		switch. This function will loop indefinately at roughly 50 Hz until the
// 		user stops the program.
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(1);
pros::Motor right_mtr(2);
okapi::MotorGroup left_motors({1,-2});
okapi::MotorGroup right_motors({-3,4});

okapi::MotorGroup lift_motors({5,6,-7,-9});
void opcontrol() {

	//startAllTasks();
    //pros::Task pos_tracking(calculatePos, NULL);




    const okapi::QLength WHEEL_DIAMETER = 3.95_in;
    const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
    const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);
    okapi::ChassisControllerPID driveController = ChassisControllerFactory::create(
            {1,-2}, {3,-4},
            okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
            okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
            okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
            ratio,
            {WHEEL_DIAMETER, CHASSIS_WIDTH}
    );


    bool arcade = false;
	bool voltageControl = false;

	while (true)
	{
		pros::lcd::print(1, "%f", ADIGyro('A').get());
        pros::lcd::print(2, "X_POS: %f", X_Pos);
        pros::lcd::print(3, "Y_POS: %f", Y_Pos);

		//driveController.driveVector(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/127.0, (0-Arduino.BNO055_Main.get())/180.0/5.0);

		//pros::lcd::print(2, "heading: %f", Arduino.BNO055_Main.get());

		/*if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
		{
			pros::lcd::print(1, "Resetting");
			Arduino.BNO055_Main.reset();
		}*/

		liftTeleop();
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
		    arcade = true;
		}
        else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
            arcade = false;
        }

        if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)){
            voltageControl = true;
        }
        else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
            voltageControl = false;
        }

        if(voltageControl) {
            if (arcade) {
                driveController.arcade((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127,
                                       (float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                       0.05);
            } else {
                driveController.tank((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                     (float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127,
                                     0.05);
            }
        }
        else{
            if (arcade) {
                left_motors.moveVelocity(((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * (200.0/127.0)) + ((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0)));
                right_motors.moveVelocity(((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * (200.0/127.0)) - ((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0)));
            } else {
                left_motors.moveVelocity((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0));
                right_motors.moveVelocity((float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * (200.0/127.0));
            }
        }

		pros::delay(20);
	}
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
