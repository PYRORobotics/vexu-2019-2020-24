#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "api.h"
#include <tuple>
#include "matrix.h"
#include "stdio.h"
#include <fstream>

#define REFRESH 0.02

class KalmanFilter1
{
private:
  double* data1;
  double* data2;

  float gain;

  double error_estimate_curr;
  double error_estimate_prev;
  double error_input;

  double estimate_curr;
  double estimate_prev;

  void calculate_gain();
  void calculate_curr_est();
  void calculate_new_error(); // Uncertainty

public:
  KalmanFilter1(double*, double* = NULL, double = 100);
  void update_kalman(void*)
  {
    while(1)
    {
      calculate_gain();
      calculate_curr_est();
      error_estimate_prev = error_estimate_curr; // Transition to next interval
      calculate_new_error();
      pros::delay(20);
    }
  }
  //pros::Task t_kalman_update;
};

class KalmanFilter
{
private:
  double* data1;
  double* data2;

  Matrix x_t;
  Matrix A;
  Matrix B;

  float gain;

  double error_estimate_curr;
  double error_estimate_prev;
  double error_input;

  double estimate_curr;
  double estimate_prev;

  void calculate_gain();
  void calculate_curr_est();
  void calculate_new_error(); // Uncertainty

public:
  KalmanFilter(double*, double* = NULL, double = 100);
  void update_kalman(void*)
  {
    while(1)
    {
      calculate_gain();
      calculate_curr_est();
      error_estimate_prev = error_estimate_curr; // Transition to next interval
      calculate_new_error();
      pros::delay(20);
    }
  }
  //pros::Task t_kalman_update;
};

enum Point3D
{
  x,
  y,
  z
};

class OrientationData
{
private:
  static double heading;
  static std::tuple <double, double, double> position;
  static std::tuple <double, double, double> velocity;
  static std::tuple <double, double, double> acceleration;
  inline static int time = 0;

public:
  OrientationData();
  inline static pros::Mutex mutex;
  static bool isOld()
  {
    if( (pros::millis() - time) < 1000 )
      return 0;
    else
      return 1;
  }
  static void setHeading(double heading)
  {
    OrientationData::heading = heading;
  };
  static void setPosition(Point3D id, double value)
  {
    switch (id) {
      case x:
        std::get<0>(OrientationData::position) = value;
        break;
      case y:
        std::get<1>(OrientationData::position) = value;
        break;
      case z:
        std::get<2>(OrientationData::position) = value;
        break;
    }
  };
  static void setPosition(double x1, double y1, double z1)
  {
    setPosition(x, x1);
    setPosition(y, y1);
    setPosition(z, z1);
  };
  static void setVelocity(Point3D id, double value)
  {
    switch (id) {
      case x:
        std::get<0>(OrientationData::velocity) = value;
        break;
      case y:
        std::get<1>(OrientationData::velocity) = value;
        break;
      case z:
        std::get<2>(OrientationData::velocity) = value;
        break;
    }
  }
  static void setAcceleration(Point3D id, double value)
  {
    switch (id) {
      case x:
        std::get<0>(OrientationData::acceleration) = value;
        break;
      case y:
        std::get<1>(OrientationData::acceleration) = value;
        break;
      case z:
        std::get<2>(OrientationData::acceleration) = value;
        break;
    }
  }
  static void reset()
  {
    setPosition(x,0.0);
    setPosition(y,0.0);
    setPosition(z,0.0);
  }
  static void setTime(int time) {OrientationData::time = time;}
  static double getTime() {return time;}
  static double getHeading() {return heading;}
  static double getPosition(Point3D id)
  {
    switch (id) {
      case x:
        return std::get<0>(position);
      case y:
        return std::get<1>(position);
      case z:
        return std::get<2>(position);
    }
  }
  double getVelocity(Point3D);
  static double getAcceleration(Point3D id)
  {
    switch (id) {
      case x:
        return std::get<0>(acceleration);
      case y:
        return std::get<1>(acceleration);
      case z:
        return std::get<2>(acceleration);
    }
  }

  static pros::Mutex mutex_position;

  static void writePosToFile(void*)
  {
    // FILE* usd_file_write = fopen("/usd/position.csv", "w");
    // fputs(" ", usd_file_write);
    // fclose(usd_file_write);
    //
    while(1)
    {
    //   FILE* usd_file_write = fopen("/usd/position.csv", "a");
    //
    //   if(usd_file_write != NULL)
    //   {
    //     // std::string str = std::to_string(heading) + "," + std::to_string(std::get<0>(acceleration)) + ","
    //     // + std::to_string(std::get<1>(acceleration)) + "," + std::to_string(std::get<2>(acceleration));
    //     // str += "\n";
    //
        std::string str = std::to_string(OrientationData::getPosition(x)) + "," + std::to_string(OrientationData::getPosition(y)) + ","
        + std::to_string(OrientationData::getHeading());
        str += "\n";

        //std::cout << " "<<str << std::endl;
    //
    //     const char *cstr = str.c_str();
    //     fputs(cstr, usd_file_write);
    //     fclose(usd_file_write);
    //   }
      pros::delay(20);
    }

  }

  pros::Task writeToFile;

};

extern OrientationData orientation;

#endif
