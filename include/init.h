#ifndef INIT_H
#define INIT_H

#include "api.h"
#include "okapi/api.hpp"

using namespace okapi;
using pros::delay;

extern void liftTeleop();

extern pros::Controller master;

extern okapi::MotorGroup lift_motors;

extern void init_chassis();

extern okapi::ChassisControllerPID driveController;
extern okapi::AsyncMotionProfileController MotionController;

#endif
