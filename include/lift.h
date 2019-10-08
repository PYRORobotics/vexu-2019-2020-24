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
        PYROLift(okapi::MotorGroup motors, int pneumaticFloorPort, int pneumaticDoorPort);
    private:
        okapi::MotorGroup liftMotors;
        int cubeCount;
        float getAngleForHeight(float inches);

};


#endif //VEXU_2019_2020_24_LIFT_H
