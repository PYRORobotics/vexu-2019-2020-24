#include "pid.h"
#include <iostream>
#include <cmath>

//==================================START FILE==================================
//==============================================================================
// File:    pid.cpp
// Author:  bradley219, Brandon Rice
// Created: 7 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// This file contains the definitions of the PIDImpl class used in
// PositionPIDController.
//
// Credits: https://gist.github.com/bradley219/5373998
//
//------------------------------------------------------------------------------


// File Setup
// ----------
using namespace std;


class PIDImpl
{
    public:
        PIDImpl( double dt, double max, double min, double Kp, double Kd, double Ki );
        ~PIDImpl();
        double calculate( double setpoint, double pv, double* );

    private:
        double _dt;
        double _max;
        double _min;
        double _Kp;
        double _Kd;
        double _Ki;
        double _pre_error;
        double _integral;
};


PIDController::PIDController( double dt, double max, double min, double Kp, double Kd, double Ki )
{
    pimpl = new PIDImpl(dt,max,min,Kp,Kd,Ki);
}
double PIDController::calculate( double setpoint, double pv, double* err )
{
    return pimpl->calculate(setpoint,pv, err);
}
PIDController::~PIDController()
{
    delete pimpl;
}


/**
 * Implementation
 */
PIDImpl::PIDImpl( double dt, double max, double min, double Kp, double Kd, double Ki ) :
    _dt(dt),
    _max(max),
    _min(min),
    _Kp(Kp),
    _Kd(Kd),
    _Ki(Ki),
    _pre_error(0),
    _integral(0)
{
}

double PIDImpl::calculate( double setpoint, double pv, double *err )
{

    // Calculate error
    double error = setpoint - pv;

    if(setpoint < 0)
    error = -error;

    // Proportional term
    double Pout = _Kp * error;

    // Integral term
    _integral += error * _dt;
    double Iout = _Ki * _integral;

    // Derivative term
    double derivative = (error - _pre_error) / _dt;
    double Dout = _Kd * derivative;

    // Calculate total output
    double output = Pout + Iout + Dout;

    // Graphs: https://www.desmos.com/calculator/cdvoh9kvus

    if(pv <= 0.2 * setpoint)
    {
      pros::lcd::print(1, "LIMITING");
      _max = 10 + 90/( 1 + exp( -40 * (pv / setpoint - 0.07) ) );
    }
    else if(pv >= 0.8 * setpoint)
    {
      pros::lcd::print(1, "LIMITING");
      _max = 100 - 90/( 1 + exp( -50 * (pv / setpoint - 0.9) ) );
    }
    else
    {
      pros::lcd::print(1, "NOT LIMITING");
    }

    pros::lcd::print(2, "max: %f", _max);

    // Restrict to max/min
    if( fabs(output) > _max )
    {
        output = _max;
}
    else if( fabs(output) < _min )
    {
      output = _min;
    }

    if(setpoint < 0)
    output = -output;


    // Save error to previous error
    _pre_error = error;

    if(err != NULL)
      *err = error;

    return output;
}

PIDImpl::~PIDImpl()
{
}







//
// PIDControllerRemake::PIDControllerRemake(double& pv, double& cv, float kp, float ki, float kd, float min, float max, float dt) : pv(pv), cv(cv), setpoint(0.0)
// {
//   setpoint = 0;
//   this->kp = kp;
//   this->ki = ki;
//   this->kd = kd;
//   this->min = min;
//   this->max = max;
//   this->dt = dt;
//
//   PIDControllerManager::controllerList.push_back(*this);
// }

PIDControllerRemake::PIDControllerRemake(float& setpoint, double& pv, double& cv, float kp, float ki, float kd, float min, float max, float dt) : pv(pv), cv(cv), setpoint(setpoint)//, id(id)
{
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->min = min;
  this->max = max;
  this->dt = dt;

  PIDControllerManager::controllerList.push_back(*this);
}

void PIDControllerRemake::setSetpoint(double setpoint)
{
  this->setpoint = setpoint;
}


void PIDControllerRemake::iterate()
{
  int speed;
  if(setpoint > 0.125)
  {
    // Ramp-up
    if(pv < 0.25*setpoint)
    {
      // std::cout << "LIMITING (Rev-up): ";
      speed = max * pow(0.8 + pow(2.71828182846,2.5-40*(pv/setpoint)), -0.75);
    }
    // PID
    else
    {
      speed = max * pow(0.8 + pow(2.71828182846,2.5-40*(pv/setpoint)), -0.75);
      int speed2 = kp * (setpoint - pv);
      if(fabs(speed2) < fabs(speed))
      {
        speed = speed2;
      }
      if(speed > max)
      {
        // std::cout << "LIMITING: ";
        speed = max;
      }
    }
    // std::cout << "speed: " << speed << " pv: " << pv << " setpt: " << setpoint << std::endl;
  }
  else
  {
    // Stop
    speed = 0;
    // PIDControllerManager.removePID(this);
  }
  cv = speed;

  // std::cout << "Speed, setpt " << speed << ", " << setpoint << std::endl;

}



PIDControllerManager::PIDControllerManager() : t_PIDManager(manager,(void*)NULL, TASK_PRIORITY_DEFAULT,
                          TASK_STACK_DEPTH_DEFAULT, "PID Manager task")
{

}




//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
