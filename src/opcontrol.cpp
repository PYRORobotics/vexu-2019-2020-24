#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */


using namespace okapi;








void startAllTasks()
{
	pros::Task BNO055_Update(t_update_BNO055);
}

void opcontrol() {

	startAllTasks();



	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);


	okapi::BNO055 mainBNO055;
	okapi::ADIGyro gyro('E');

	const okapi::QLength WHEEL_DIAMETER = 3.95_in;
	const okapi::QLength CHASSIS_WIDTH = 16.5_in;//13.9_in;//14.19_in;//13.625_in;
	const okapi::AbstractMotor::GearsetRatioPair ratio = okapi::AbstractMotor::gearset::green;// * (1.0382);

	//auto controller = okapi::AsyncControllerFactory::posPID({-1, 2}, mainBNO055, 0.001, 0.0, 0.0001);
	auto driveController = ChassisControllerFactory::create(
	   {1,1}, {2,2},
	   okapi::IterativePosPIDController::Gains{0.00001, 0.00001, 0.000006},   //straight
	   okapi::IterativePosPIDController::Gains{0.000, 0.0, 0.0000},    //correct drift
	   okapi::IterativePosPIDController::Gains{0.001, 0.00001, 0.00000},  //turn
	   ratio,
	   {WHEEL_DIAMETER, CHASSIS_WIDTH}
	 );


	double i = 0;
	while (true)
	{

			driveController.driveVector(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)/127.0, (0-gyroValue)/180.0);

		pros::lcd::print(2, "heading: %f", mainBNO055.controllerGet(gyroValue));
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			pros::lcd::print(1, "GO!");
			//controller.setTarget(200);
			driveController.stop();
			pros::delay(20000000);
		}
		pros::delay(20);
	}
}
