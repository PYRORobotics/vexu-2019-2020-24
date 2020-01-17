#ifndef _PISTON_H_
#define _PISTON_H_

#include "api.h"
#include "arduino.h"

//==================================START FILE==================================
//==============================================================================
// File:    piston.h
// Author:  Brandon Rice
// Created: 17 October 2019
// Last Modified: 17 October 2019
//
// Description:
// ------------
// The header file for piston.cpp, containing the class declaration of Piston.
//
//------------------------------------------------------------------------------

class Piston
{
private:
  int port;
  pros::ADIDigitalOut adi;
  int8_t state;

public:
  Piston(char, int8_t = 0);
  void extend();
  void retract();
  void set(bool);
  void toggle();
};

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
