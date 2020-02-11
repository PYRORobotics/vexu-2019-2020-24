#ifndef _PID_H_
#define _PID_H_

#include "api.h"
#include <vector>
#include <algorithm>
#include "okapi/api/filter/averageFilter.hpp"


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


struct PIDReturn
{
  int left, right;
};

class PIDControllerRemake
{
  private:
    float kp, ki, kd, min, max, dt;
    float& setpoint;
    double tollerance;
    double& pv;
    double& cv;
    double prev_error;
    double integral;
    okapi::AverageFilter<3> avgFilterError;


    // const int id;

    void iterate();

  public:
    // PIDControllerRemake(double&, double&, float, float, float, float, float, float = 333);
    PIDControllerRemake(float&, double&, double&, float, float, float, float, float, double = 0.125, float = 333);//, int = PIDControllerManager.num_objects++);

    void setSetpoint(double setpoint);
    friend class PIDControllerManager;
    double avgError();

};

class PIDControllerManager
{
private:
  static void manager(void*)
  {
    while(1)
    {
      for(auto PID : controllerList)
        PID.iterate();
      pros::delay(20);
    }
  }
  inline static std::vector<PIDControllerRemake> controllerList;

  pros::Task t_PIDManager;

public:
  PIDControllerManager();
  friend class PIDControllerRemake;
  inline static int num_objects = 0;
  static void removePID(PIDControllerRemake& P)
  {
    // int pos = -1;
    // for (int i = 0; i < controllerList.size(); i++) {
    //   if (controllerList.data()->id == P.id)
    //     pos = i;
    // }
    //
    // if(pos >= 0)
    // {
    //   controllerList.erase(controllerList.begin() + pos);
    //
    //   num_objects--;
    //   std::cout << "Removed a PID at position: " << pos << std::endl;
    // }

  }

};

#endif
