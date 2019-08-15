#include "robot.hpp"


Robot::Robot() :  left_mtr(1,pros::E_MOTOR_GEARSET_18,0,pros::E_MOTOR_ENCODER_ROTATIONS),
                  right_mtr(2,pros::E_MOTOR_GEARSET_18,1,pros::E_MOTOR_ENCODER_ROTATIONS),
                  PositionPIDController(20, 100, -100, 0.5, 0, 0)
{
  // std::pair<double, double> position = {0.0,0.0};
  // state_estimate_position = {position, 0.0};
  Robot24.last_motor_left = Robot24.left_mtr.get_position();
  Robot24.last_motor_right = Robot24.right_mtr.get_position();

  Robot24.telemetry.set_x(0.0);
  Robot24.telemetry.set_y(0.0);
  Robot24.telemetry.set_heading(0.0);
  left_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  right_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

// Robot::Robot(double* heading)
// {
//   // std::pair<double, double> position = {0.0,0.0};
//   // state_estimate_position = {position, *heading};
// }

// void Robot::set_state_estimate_position(double x, double y, double heading)
// {
//   state_estimate_position = std::make_tuple(std::make_pair(x, y), heading);
// }
// void Robot::set_state_estimate_position(std::tuple<std::pair<double, double>, double> state_estimate_position)
// {
//   this->state_estimate_position = state_estimate_position;
// }


Robot Robot24;


void t_update_telemetry(void*)
{
  while(1)
  {
    Robot24.telemetry.set_heading(BNO055_Main.get());
    Robot24.telemetry.set_x(Robot24.telemetry.x + (Robot24.left_mtr.get_position() - Robot24.last_motor_left)/8/M_PI * sin(radians(Robot24.telemetry.heading)));
    Robot24.telemetry.set_y(Robot24.telemetry.y + (Robot24.right_mtr.get_position() - Robot24.last_motor_right)/8/M_PI * cos(radians(Robot24.telemetry.heading)));
    Robot24.telemetry.last_timestamp = pros::millis();

    pros::lcd::print(5, "L,R:%f,%f:%f,%f", Robot24.left_mtr.get_position(), Robot24.right_mtr.get_position(),Robot24.last_motor_left,Robot24.last_motor_right);
    pros::lcd::print(6, "XYHT:%f,%f,%f@%d", Robot24.telemetry.x,Robot24.telemetry.y,Robot24.telemetry.heading, Robot24.telemetry.last_timestamp);

    Robot24.last_motor_left = Robot24.left_mtr.get_position();
    Robot24.last_motor_right = Robot24.right_mtr.get_position();

    pros::delay(5);
  }
}

pros::Task update_telemetry(t_update_telemetry);
