#include "main.h"
#include <math.h>
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

// void u_ultrasonic_csv(void*)
// {
//
// 	// std::string	profile_file_name = std::to_string(pros::millis());
//
//
//   std::string line;
//   char filepath[1000];
//   strcpy(filepath, "/usd/usdata2");
//   // strcat(filepath, profile_file_name.c_str());
//   strcat(filepath, ".csv");
//   FILE* save_file;
// 	//save_file = fopen(filepath, "w");
//   //fputs("USLeft, USFront, USRight, USLeft Cartesian X, USLeft Cartesian Y, USFront Cartesian X, USFront Cartesian Y, USRight Cartesian X, USRight Cartesian Y, XEst, YEst, Heading\n", save_file);
// 	//fclose(save_file);
// 	  while(1){
// 	    //std::cout << "Start saving!!!\n";
// 	    save_file = fopen(filepath, "a");
//
// 			int usleftvalue = Ultrasonic_Left.get_value();
// 			int usfrontvalue = Ultrasonic_Front.get_value();
// 			int usrightvalue = Ultrasonic_Right.get_value();
//
//
// 			char value[5];
// 			itoa(usleftvalue, value, 10);
//       fputs(value, save_file);
//       fputs(",", save_file);
//
// 			itoa(usfrontvalue, value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa(usrightvalue, value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			if(usleftvalue > 30 && usleftvalue < 2500)
// 			{
// 			itoa((int)(-(usleftvalue + in_to_mm(9)) * sin(radians(Robot24.telemetry.heading)) + Robot24.telemetry.x), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa((int)(-(usleftvalue + in_to_mm(9)) * cos(radians(Robot24.telemetry.heading)) + Robot24.telemetry.y), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
// 		}
// 		else{fputs(",", save_file);
// 	fputs(",", save_file);}
//
//
// 		if(usfrontvalue > 30 && usfrontvalue < 2500)
// 		{
// 			itoa((int)(-(usfrontvalue + in_to_mm(9)) * sin(radians(Robot24.telemetry.heading)) + Robot24.telemetry.x), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa((int)(-(usfrontvalue + in_to_mm(9)) * cos(radians(Robot24.telemetry.heading)) + Robot24.telemetry.y), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
// 		}
// 		else{fputs(",", save_file);fputs(",", save_file);}
//
// 		if(usrightvalue > 30 && usrightvalue < 2500)
// 		{
//
// 			itoa((int)(-(usrightvalue + in_to_mm(9)) * sin(radians(Robot24.telemetry.heading)) + Robot24.telemetry.x), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa((int)(-(usrightvalue + in_to_mm(9)) * cos(radians(Robot24.telemetry.heading)) + Robot24.telemetry.y), value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
// 		}
// 		else{fputs(",", save_file);fputs(",", save_file);}
//
// 			itoa((int)Robot24.telemetry.x, value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa((int)Robot24.telemetry.y, value, 10);
// 			fputs(value, save_file);
// 			fputs(",", save_file);
//
// 			itoa((int)Robot24.telemetry.heading, value, 10);
// 			fputs(value, save_file);
// 	    fputs("\n", save_file);
//
//
// 	    fclose(save_file);
// 			pros::delay(200);
//
// 	  }
//
//
// 	}
//
//
//
// // FIXME - Add Header?
// void startAllTasks()	//FIXME
// {
// 	pros::Task us_task(u_ultrasonic_csv);
// }


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
void opcontrol() {

	//startAllTasks();

	pros::Controller master(pros::E_CONTROLLER_MASTER);

	const okapi::QLength WHEEL_DIAMETER = 3.95_in;
	const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
	const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);

	//auto controller = okapi::AsyncControllerFactory::posPID({-1, 2}, BNO055_Main, 0.001, 0.0, 0.0001);
	auto driveController = ChassisControllerFactory::create(
	   {1,1}, {-2,-2},
	   okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
	   okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
	   okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
	   ratio,
	   {WHEEL_DIAMETER, CHASSIS_WIDTH}
	 );

/*
	while (true)
	{
		//driveController.driveVector(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/127.0, (0-BNO055_Main.get())/180.0*1.8);

		driveController.arcade(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/127.0, master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)/127.0);

		pros::lcd::print(2, "heading: %f", BNO055_Main.get());

		pros::lcd::print(4, "US: %d", Ultrasonic_Left.get_value());


		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
		{
			pros::lcd::print(1, "Resetting");
			BNO055_Main.reset();
		}

		pros::delay(20);
	}*/
	while(pros::millis() < 10000)
	{
		int output = (int)Robot24.PositionPIDController.calculate(3600/10/PI*10, Robot24.left_mtr.get_position());
		pros::lcd::print(3, "Output: %d", output);
		Robot24.left_mtr = output;
		pros::delay(20);
	}

	Robot24.left_mtr.tare_position();

	while(pros::millis() < 20000)
	{
		int output = (int)Robot24.PositionPIDController.calculate(3600/2/PI*10, Robot24.left_mtr.get_position());
		pros::lcd::print(3, "Output: %d", output);
		Robot24.left_mtr = output;
		pros::delay(20);
	}
	Robot24.left_mtr.tare_position();
	while(pros::millis() < 30000)
	{
		int output = (int)Robot24.PositionPIDController.calculate(3600/4/PI*10, Robot24.left_mtr.get_position());
		pros::lcd::print(3, "Output: %d", output);
		Robot24.left_mtr = output;
		pros::delay(20);
	}

}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
