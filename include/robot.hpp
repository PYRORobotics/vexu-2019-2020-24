#ifndef _ROBOT_HPP_
#define _ROBOT_HPP_

#include "api.h"
#include <functional>
#include "telemetry.hpp"
#include "bno055.hpp"
#include "math.hpp"
#include "pid.hpp"

extern void t_update_telemetry(void*);

class Robot {
  private:
    // std::tuple<std::pair<double, double>, double> state_estimate_position;

  public:
    Robot();
    Robot(double* heading);
    Telemetry telemetry;
    pros::Motor left_mtr;
    pros::Motor right_mtr;
    double last_motor_left;
    double last_motor_right;
    //
    // double value;
    // double setpoint;
    // double* measured_value;
    // double* position_setpoint;
    //
    PIDController PositionPIDController;

    // void set_state_estimate_position(double, double, double);
    // void set_state_estimate_position(std::tuple<std::pair<double, double>, double>);


};

extern Robot Robot24;

#endif
