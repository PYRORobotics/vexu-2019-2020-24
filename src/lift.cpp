//
// Created by charles on 9/30/19.
//
#include "main.h"
int liftTarget = 10;
void liftTeleop(){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        liftTarget += 25;
    }
    else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        liftTarget -= 25;
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        lift_motors.moveVelocity(50);
        liftTarget = lift_motors.getPosition();
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        lift_motors.moveVelocity(-50);
        liftTarget = lift_motors.getPosition();
    }
    else{
        lift_motors.moveAbsolute(liftTarget, 50);
    }


    //delay(20);
}