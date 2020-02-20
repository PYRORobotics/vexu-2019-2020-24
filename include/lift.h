#ifndef _LIFT_H_
#define _LIFT_H_

#include "piston.h"
#include "okapi/api.hpp"
#include "arduino.h"

//==================================START FILE==================================
//==============================================================================
// File:    lift.h
// Author:  Charles Jeffries, Brandon Rice
// Created: 7 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// The header file for lift.cpp, containing the class declaration of PYROLift.
//
//------------------------------------------------------------------------------


class PYROLift {
    public:
        void loopTeleop();
        float getMotorTemps();
        PYROLift(int motorTopRight, int motorBottomRight, int motorTopLeft, int motorBottomLeft, int pneumaticFloorPort, int pneumaticDoorPort);
        float getLiftHeight();
        float getLiftAngle();
        float getAngleForHeight(float inches);
        float getMotorDegreesFromLiftDegrees(float degrees);
        float getMotorPos();
        void moveLiftToHeight(float inches, int maxVelocity);
        void tare();
        void collectCube(bool tareOverride = false);
        void collectCube(int);
        void intakeAndCollect();
        Piston piston_door;
        Piston piston_floor;
        okapi::ADIButton LimitSwitch;

        static int liftTarget;
        static void auton(void*)
        {
            liftMotors.tarePosition();
          while(1)
          {
            liftMotors.moveAbsolute(liftTarget, 50);
            pros::delay(20);
          }
        }

    private:
        static okapi::MotorGroup liftMotors;
        int cubeCount;
        void manualControl();
};

extern PYROLift lift;

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================

#endif
