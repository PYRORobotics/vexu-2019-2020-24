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

PIDControllerRemake::PIDControllerRemake(float& setpoint, double& pv, double& cv, float kp, float ki, float kd, float min, float max, double tollerance, float dt) : pv(pv), cv(cv), setpoint(setpoint)//, id(id)
{
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->min = min;
  this->max = max;
  this->dt = dt;
  this->tollerance = tollerance;
  prev_error = 0;
  integral = 0;

  for(int i = 0; i < 50; i++)
  {
    avgFilterError.filter(48);
  }

  PIDControllerManager::controllerList.push_back(*this);
}

void PIDControllerRemake::setSetpoint(double setpoint)
{
  this->setpoint = setpoint;
}

double PIDControllerRemake::avgError()
{
  return avgFilterError.getOutput();
}

void PIDControllerRemake::iterate()
{
  float speed;
  float slew = 0.7;
  if(fabs(setpoint) > fabs(tollerance))
  {
    // Ramp-up
    if(pv < 0.25*setpoint)
    {
      // std::cout << "LIMITING (Rev-up): ";
      // speed = max * pow(0.8 + pow(2.71828182846,2.5-40*(pv/setpoint)), -0.75);
      // https://www.desmos.com/calculator/8kwtgeelos
      speed = max * pow(1 + pow(2.5*slew/max, 2.5-20 * pv),-0.75);
    }
    // PID
    else
    {
      speed = max * pow(1 + pow(2.5*slew/max, 2.5-20 * pv),-0.75);
      setpoint < 0? speed *= -1: 0;
      // float speedPID = kp * (setpoint - pv);

      // Calculate error
      double error = setpoint - pv;

      avgFilterError.filter(error);

      if(setpoint < 0)
      error = -error;

      // Proportional term
      double Pout = kp * error;

      // Integral term
      integral += error * dt;
      double Iout = ki * integral;

      // Derivative term
      double derivative = (error - prev_error) / dt;
      double Dout = kd * derivative;

      // Calculate total output
      float speedPID = Pout + Iout + Dout;
      prev_error = error;


      if(speedPID < speed)
      {
        speed = speedPID;
      }
      if(abs(speed) > max)
      {
        // std::cout << "LIMITING: ";
        speed < 0? speed = -max : speed = max;
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
