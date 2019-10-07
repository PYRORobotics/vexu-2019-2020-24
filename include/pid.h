#ifndef _PID_H_
#define _PID_H_

#include "api.h"

// From: https://gist.github.com/bradley219/5373998

/*
previous_error = 0
integral = 0
loop:
  error = setpoint - measured_value
  integral = integral + error * dt
  derivative = (error - previous_error) / dt
  output = Kp * error + Ki * integral + Kd * derivative
  previous_error = error
  wait(dt)
  goto loop
*/

struct PIDData
{
  double error;
  double target_position;
};

class PIDImpl;
class PIDController
{
    public:
        // dt -  loop interval time
        // max - maximum value of manipulated variable
        // min - minimum value of manipulated variable
        // Kp -  proportional gain
        // Ki -  Integral gain
        // Kd -  derivative gain
        PIDController( double dt, double max, double min, double Kp, double Kd, double Ki );

        // Returns the manipulated variable given a setpoint and current process value
        double calculate( double setpoint, double pv, double *error_ptr );
        ~PIDController();

    private:
        PIDImpl *pimpl;
};


#endif
