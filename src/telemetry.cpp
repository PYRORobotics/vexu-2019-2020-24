#include "telemetry.hpp"


Telemetry::Telemetry()
{
  set_x(0);
  set_y(0);
  set_heading(0);
  last_timestamp = pros::millis();
}

void Telemetry::set_x(double x)
{
  x_ = x;
}
void Telemetry::set_y(double y)
{
  y_ = y;
}
void Telemetry::set_heading(double heading)
{
  heading_ = heading;
}
