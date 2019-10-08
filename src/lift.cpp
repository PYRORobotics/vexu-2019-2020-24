//
// Created by charles on 10/7/19.
//

#include "lift.h"

PYROLift::PYROLift(int motorTopRight, int motorBottomRight, int motorTopLeft, int motorBottomLeft, int pneumaticFloorPort, int pneumaticDoorPort){
    liftMotors = okapi::MotorGroup({});
}