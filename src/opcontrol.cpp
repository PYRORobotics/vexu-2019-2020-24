#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		opcontrol.cpp
// Author:	Brandon Rice, Charles Jefferies
// Created: 13 July 2019
// Last Modified: 29 October 2019
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
//		Nonepros::Controller master
//```
//------------------------------------------------------------------------------
pros::Controller master(pros::E_CONTROLLER_MASTER);

void liftTask(void*){
    lift.loopTeleop();
}
void opcontrol() {

    bool arcade = false;
	bool voltageControl = false;


	okapi::ADIEncoder LEFT('A', 'B', true);
	okapi::ADIEncoder RIGHT('C', 'D');
    pros::Task lifttaskteleop(liftTask, (void*)NULL, TASK_PRIORITY_DEFAULT,
                            TASK_STACK_DEPTH_DEFAULT, "lift teleop task");
	while (true)
	{


		printf("%f %f\n", LEFT.get(), RIGHT.get());
        chassis.driveController.tank((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                     (float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127,
                                     0.05);
        //lift.loopTeleop();

		//driveController.driveVector(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/127.0, (0-Arduino.BNO055_Main.get())/180.0/5.0);

		//pros::lcd::print(2, "heading: %f", Arduino.BNO055_Main.get());

		/*if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
		{
			pros::lcd::print(1, "Resetting");
			Arduino.BNO055_Main.reset();
		}*/
        /*
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
		    arcade = true;
				Arduino.BNO055_Main.reset();
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
                chassis.driveController.arcade((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) / 127,
                                       (float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                       0.05);
            } else {
                chassis.driveController.tank((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                     (float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127,
                                     0.05);
            }
        }
        else{
            // if (arcade) {
            //     left_motors.moveVelocity(((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * (200.0/127.0)) + ((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0)));
            //     right_motors.moveVelocity(((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * (200.0/127.0)) - ((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0)));
            // } else {
            //     left_motors.moveVelocity((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * (200.0/127.0));
            //     right_motors.moveVelocity((float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * (200.0/127.0));
            // }
        }
        */
		pros::delay(20);
	}
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
