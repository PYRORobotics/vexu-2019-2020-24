#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "api.h"
#include <tuple>
#include "matrix.h"

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

public:
  OrientationData();
  void setHeading(double);
  void setPosition(Point3D, double);
  void setVelocity(Point3D, double);
  void setAcceleration(Point3D, double);
  double getHeading();
  double getPosition(Point3D);
  double getVelocity(Point3D);
  double getAcceleration(Point3D);
};

extern OrientationData orientation;

#endif
