#include "math.hpp"
#include <math.h>

double radians(double degrees)
{
  return degrees * M_PI / 180.0;
}

double in_to_mm(double in)
{
  return in * 25.4;
}
