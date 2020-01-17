#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:    piston.cpp
// Author:  Brandon Rice
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
// Method: Piston(char, int8_t) :
// ------------------------------
// Description:
//     Constructs a Piston (pneumatic acturator) object.
//
// Parameters:
//```
//    char port - the 3-wire port (A-H) the piston is plugged into.
//    int8_t starting_state - whether the piston starts retracted or extended.
//```
// Objects to Initialize:
//```
//    adi (pros::ADIDigitalOut)
//```
//------------------------------------------------------------------------------
Piston::Piston(char port, int8_t starting_state): adi(port)
{
  this->port = port;
  state = starting_state;
}


//------------------------------------------------------------------------------
// Method: extend() :
// ------------------
// Description:
//     Extends the piston.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void Piston::extend()
{
  if(port > 8)
    adi.set_value(1);
  else
    PYRO_Arduino::setADI(port,1);
  state = 1;
}


//------------------------------------------------------------------------------
// Method: retract() :
// -------------------
// Description:
//     Retracts the piston.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void Piston::retract()
{
  if(port > 8)
    adi.set_value(0);
  else
    PYRO_Arduino::setADI(port,0);
  state = 0;
}


//------------------------------------------------------------------------------
// Method: set(bool) :
// -------------------
// Description:
//     Sets the piston to be extended (1) or retracted (0).
//
// Parameters:
//```
//    bool output - extended (1) or retracted (0)
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void Piston::set(bool output)
{
  if(port > 8)
    adi.set_value(output);
  else
    PYRO_Arduino::setADI(port,output);
  state = output;
}


//------------------------------------------------------------------------------
// Method: toggle() :
// ------------------
// Description:
//     Toggles the piston between being extended or retracted.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void Piston::toggle()
{
  if(state == 1)
  {
    if(port > 8)
      adi.set_value(0);
    else
      PYRO_Arduino::setADI(port,0);
    state = 0;
  }
  else
  {
    if(port > 8)
      adi.set_value(0);
    else
      PYRO_Arduino::setADI(port,0);
    state = 1;
  }
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
