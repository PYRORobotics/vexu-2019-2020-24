#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_
#include "api.h"

//==================================START FILE==================================
//==============================================================================
// File:    autonomous.h
// Author:  Brandon Rice
// Created: 13 July 2019
// Last Modified: 30 October 2019
//
// Description:
// ------------
// The header file for autonomous.cpp, containing functions to run autonomous
// routines.
//
//------------------------------------------------------------------------------

enum autonNames
{
  notSelected,
  red0,
  blue0,
  red1,
  blue1,
  redSkills0,
  blueSkills0
};

// typedef autonNames autonID;

// static autonNames selectedAutonID = notSelected;

extern void autonomousSelector(autonNames);
extern void auton_red0();
extern void auton_blue0();
extern void auton_red1();
extern void auton_blue1();
extern void auton_skills0();
extern pros::Task lifttaskauto;

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
