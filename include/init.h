#ifndef INIT_H
#define INIT_H

#include "api.h"
#include "okapi/api.hpp"

using namespace okapi;

extern void init_chassis();

extern okapi::ChassisControllerPID driveController;
extern okapi::AsyncMotionProfileController MotionController;

#endif
