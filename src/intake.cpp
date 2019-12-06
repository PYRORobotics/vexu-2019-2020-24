#include "intake.h"

//==================================START FILE==================================
//==============================================================================
// File:    intake.cpp
// Author:  Brandon Rice
// Created: 28 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// This file contains the definitions of the PYROIntake class.
//
//------------------------------------------------------------------------------


// File Setup
// ----------
using namespace okapi;


// Global/Static Objects
// ---------------------
PYROIntake intake;


// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROIntake() :
// ----------------------
// Description:
//     Constructs a PYROIntake object and initialize child motor groups.
//
// Parameters:
//```
//    NONE
//```
// Objects to Initialize:
//```
//    motors (okapi::MotorGroup),
//```
//------------------------------------------------------------------------------

PYROIntake::PYROIntake() : motors({Motor(6, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees),
                    Motor(15, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees)})
{}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
