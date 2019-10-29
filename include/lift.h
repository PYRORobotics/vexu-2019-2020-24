//
// Created by charles on 10/7/19.
//

#ifndef _LIFT_H_
#define _LIFT_H_

#include "piston.h"
#include "okapi/api.hpp"

//#include <okapi/impl/device/motor/motorGroup.hpp>

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
        void collectCube();
        void intakeAndCollect();
        Piston piston_door;
        Piston piston_floor;
        static int liftTarget;
        static void auton(void*)
        {
          while(pros::competition::is_autonomous())
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


#endif //VEXU_2019_2020_24_LIFT_H
