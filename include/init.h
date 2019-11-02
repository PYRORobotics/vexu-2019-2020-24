#ifndef INIT_H
#define INIT_H

#include "api.h"
#include "okapi/api.hpp"

//==================================START FILE==================================
//==============================================================================
// File:    init.h
// Author:  Brandon Rice
// Created: 7 October 2019
//
// Description:
// ------------
// A header file containing many constants and other values that should be
// initialized.
//
//------------------------------------------------------------------------------

#define M_CHASSIS_LF 8
#define M_CHASSIS_LM 9
#define M_CHASSIS_LR 10
#define M_CHASSIS_RF 18
#define M_CHASSIS_RM 19
#define M_CHASSIS_RR 20
#define M_INTAKE_L 6
#define M_INTAKE_R 15


using namespace okapi;

extern void init_chassis();

extern okapi::ChassisControllerPID driveController;
extern okapi::AsyncMotionProfileController MotionController;

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
