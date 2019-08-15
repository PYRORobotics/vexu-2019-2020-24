#include "pid.hpp"

#include <iostream>
#include <cmath>

using namespace std;

class PIDImpl
{
    public:
        PIDImpl( double dt, double max, double min, double Kp, double Kd, double Ki );
        ~PIDImpl();
        double calculate( double setpoint, double pv );

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
double PIDController::calculate( double setpoint, double pv )
{
    return pimpl->calculate(setpoint,pv);
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

double PIDImpl::calculate( double setpoint, double pv )
{

    // Calculate error
    double error = setpoint - pv;

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
    if( output > _max )
        output = _max;
    else if( output < _min )
        output = _min;

    // Save error to previous error
    _pre_error = error;

    return output;
}

PIDImpl::~PIDImpl()
{
}
