#ifndef INIT_H
#define INIT_H

#include "api.h"
#include "okapi/api.hpp"

#define M_CHASSIS_LF 8
#define M_CHASSIS_LM 9
#define M_CHASSIS_LR 10
#define M_CHASSIS_RF 18
#define M_CHASSIS_RM 19
#define M_CHASSIS_RR 20

using namespace okapi;

extern void init_chassis();

extern okapi::ChassisControllerPID driveController;
extern okapi::AsyncMotionProfileController MotionController;

#endif
