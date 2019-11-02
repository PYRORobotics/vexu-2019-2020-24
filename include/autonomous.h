#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_

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
  red0,
  blue0,
  skills0
};

typedef autonNames autonID;

static autonID selectedAutonID;

extern void autonomousSelector(autonID);
extern void auton_red0();
extern void auton_blue0();
extern void auton_skills0();

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
