//
// Created by charles on 9/30/19.
//
#include "main.h"
int liftTarget = 10;

pros::ADIDigitalOut pistonFloor (7);
pros::ADIDigitalOut pistonDoor (8);

bool floorActivated = false;
bool doorActivated = false;
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

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
        doorActivated = !doorActivated;
        pistonDoor.set_value(doorActivated);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
        floorActivated = !floorActivated;
        pistonFloor.set_value(floorActivated);
    }


    //delay(20);
}