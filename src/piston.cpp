#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		piston.cpp
// Author:	Brandon Rice
// Created: 17 October 2019
// Last Modified: 17 October 2019
//
// Description:
// ------------
// This file contains the definitions of the Piston class.
//
//------------------------------------------------------------------------------


// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROChassis() :
// ----------------------------
// Description:
// 		Constructs a PYROChassis object containing a Position PID Controller and
//    other okapi chassis objects.
//
// Parameters:
//```
//		NONE
//```
// Objects to Initialize:
//```
//		PositionPIDController (PIDController),
//    left_motors (okapi::MotorGroup),
//    right_motors (okapi::MotorGroup),
//    driveController (okapi::ChassisControllerPID),
//    MotionController (okapi::AsyncMotionProfileController)
//    N/A: t_update_differential_pos (pros::Task)
//```
//------------------------------------------------------------------------------
Piston::Piston(char port, int8_t starting_state): adi(port)
{
  this->port = port;
  state = starting_state;
}

void Piston::extend()
{
  adi.set_value(1);
  state = 1;
}

void Piston::retract()
{
  adi.set_value(0);
  state = 0;
}

void Piston::set(bool output)
{
  adi.set_value(output);
  state = output;
}

void Piston::toggle()
{
  if(state == 1)
  {
    adi.set_value(0);
    state = 0;
  }
  else
  {
    adi.set_value(1);
    state = 1;
  }
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
