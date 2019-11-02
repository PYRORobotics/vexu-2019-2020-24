#ifndef _INTAKE_H_
#define _INTAKE_H_

#include "api.h"
#include "okapi/api.hpp"
#include "init.h"

//==================================START FILE==================================
//==============================================================================
// File:    intake.h
// Author:  Brandon Rice
// Created: 28 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// The header file for intake.cpp, containing the class declaration of
// PYROIntake.
//
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
