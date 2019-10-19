#ifndef INIT_H
#define INIT_H

#include "api.h"
#include "okapi/api.hpp"

#define M_CHASSIS_LF 8
#define M_CHASSIS_LR 9
#define M_CHASSIS_RF 18
#define M_CHASSIS_RR 19


using namespace okapi;

extern void init_chassis();

extern okapi::ChassisControllerPID driveController;
extern okapi::AsyncMotionProfileController MotionController;

#endif
