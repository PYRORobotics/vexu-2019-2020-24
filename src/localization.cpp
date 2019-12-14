#include "localization.h"

void KalmanFilter1::calculate_gain()
{
  gain = error_estimate_curr / (error_estimate_curr + error_input);
}

void KalmanFilter1::calculate_curr_est()
{
  estimate_curr = estimate_prev + gain * (*data1 - estimate_prev);
}

void KalmanFilter1::calculate_new_error()
{
  error_estimate_curr = (1 - gain) * error_estimate_prev;
}

KalmanFilter1::KalmanFilter1(double* data1, double* data2, double init_est)
{
  this->data1 = data1;
  this->data2 = data2;
  estimate_prev = init_est;
  error_estimate_curr = init_est;
  error_estimate_prev = init_est;
  error_input = init_est;
}

KalmanFilter::KalmanFilter(double* data1, double* data2, double init_est) : x_t(2,1), A(2,2), B(2,2)
{
  this->data1 = data1;
  this->data2 = data2;
  estimate_prev = init_est;
  error_estimate_curr = init_est;
  error_estimate_prev = init_est;
  error_input = init_est;


  A.array[0][0] = 1;
  A.array[1][1] = 1;
  A.array[0][1] = REFRESH;
}



double OrientationData::heading;
std::tuple <double, double, double> OrientationData::position;
std::tuple <double, double, double> OrientationData::velocity;
std::tuple <double, double, double> OrientationData::acceleration;



OrientationData::OrientationData()
{
  heading = 0;

  std::get<0>(position) = 0;
  std::get<1>(position) = 0;
  std::get<2>(position) = 0;

  std::get<0>(velocity) = 0;
  std::get<1>(velocity) = 0;
  std::get<2>(velocity) = 0;

  std::get<0>(acceleration) = 0;
  std::get<1>(acceleration) = 0;
  std::get<2>(acceleration) = 0;
}

void OrientationData::setHeading(double heading)
{
  this->heading = heading;
}

void OrientationData::setPosition(Point3D id, double value)
{
  switch (id) {
    case x:
      std::get<0>(position) = value;
      break;
    case y:
      std::get<1>(position) = value;
      break;
    case z:
      std::get<2>(position) = value;
      break;
  }
}

void OrientationData::setVelocity(Point3D id, double value)
{
  switch (id) {
    case x:
      std::get<0>(velocity) = value;
      break;
    case y:
      std::get<1>(velocity) = value;
      break;
    case z:
      std::get<2>(velocity) = value;
      break;
  }
}

void OrientationData::setAcceleration(Point3D id, double value)
{
  switch (id) {
    case x:
      std::get<0>(acceleration) = value;
      break;
    case y:
      std::get<1>(acceleration) = value;
      break;
    case z:
      std::get<2>(acceleration) = value;
      break;
  }
}

double OrientationData::getHeading()
{
  return heading;
}

double OrientationData::getPosition(Point3D id)
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

double OrientationData::getVelocity(Point3D id)
{
  switch (id) {
    case x:
      return std::get<0>(velocity);
    case y:
      return std::get<1>(velocity);
    case z:
      return std::get<2>(velocity);
  }
}

double OrientationData::getAcceleration(Point3D id)
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

OrientationData orientation;
