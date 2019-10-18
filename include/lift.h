//
// Created by charles on 10/7/19.
//

#ifndef VEXU_2019_2020_24_LIFT_H
#define VEXU_2019_2020_24_LIFT_H


#include <okapi/impl/device/motor/motorGroup.hpp>

class PYROLift {
    public:
        void loopTeleop();
        float getMotorTemps();
        PYROLift(int motorTopRight, int motorBottomRight, int motorTopLeft, int motorBottomLeft, int pneumaticFloorPort, int pneumaticDoorPort);
        float getLiftHeight();
    private:
        okapi::MotorGroup liftMotors;
        int cubeCount;
        float getAngleForHeight(float inches);
        void moveLiftToHeight(float inches, int maxVelocity);
        float getMotorDegreesFromLiftDegrees(float degrees);
        void stackCube();



};


#endif //VEXU_2019_2020_24_LIFT_H
