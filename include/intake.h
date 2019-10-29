#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "api.h"
#include "okapi/api.hpp"
#include "init.h"


namespace okapi
{

  class PYROIntake {
    private:



    public:
      PYROIntake();
      okapi::MotorGroup motors;

    };
}

extern okapi::PYROIntake intake;


#endif
