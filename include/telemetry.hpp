#ifndef _TELEMETRY_HPP_
#define _TELEMETRY_HPP_

#include "api.h"

class Telemetry
{
  private:
    double x_;
    double y_;
    double heading_;
  public:
    const double& x=x_;
    const double& y=y_;
    const double& heading=heading_;
    uint32_t last_timestamp;

    void set_x(double x);
    void set_y(double y);
    void set_heading(double heading);

    Telemetry();
};

#endif
