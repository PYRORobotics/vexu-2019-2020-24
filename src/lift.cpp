//
// Created by charles on 10/7/19.
//

#include "main.h"
//degrees for lift to go down to to pick up cube from ground
const float PICKUP_POSITION = 4.0;
const int maxVelocity = 20;
const float INCHES_PIVOT_TO_PIVOT = 16.0;
const float LIFT_GEAR_RATIO = 7.0;
const int maxVelocityDown = maxVelocity;
//const float TICKS_PER_MOTOR_DEGREE = 900/360; //green cartdriges
const float TICKS_PER_MOTOR_DEGREE = 1.0; //green cartdriges
const float STARTING_DEGREES = 40.5;
const float HEIGHT_OFFSET = (2*(INCHES_PIVOT_TO_PIVOT * (-cos((42.0 * pi/180)) + 1)/1)) - 2.0; //the minus 7.5 is the height addition caused by the middle tower
int liftTarget = 10;

float degreesToRadians(float degrees){

}

PYROLift::PYROLift(
        int motorTopRight,
        int motorBottomRight,
        int motorTopLeft,
        int motorBottomLeft,
        int pneumaticFloorPort,
        int pneumaticDoorPort) :
        liftMotors({
            Motor(motorTopRight, false, AbstractMotor::gearset::red),
            Motor(motorBottomRight, false, AbstractMotor::gearset::red),
            Motor(motorTopLeft, true, AbstractMotor::gearset::red),
            Motor(motorBottomLeft, true, AbstractMotor::gearset::red)}
            ){

    cubeCount = 0;


}

float PYROLift::getAngleForHeight(float inches){
    //return ((asin(((inches - HEIGHT_OFFSET)/2)/INCHES_PIVOT_TO_PIVOT)) * 180/pi)/1 + 45;
    return acos(-((inches + HEIGHT_OFFSET)/(2 * INCHES_PIVOT_TO_PIVOT)) + 1) * 180/pi;
}

float PYROLift::getMotorDegreesFromLiftDegrees(float degrees){
    return (degrees-STARTING_DEGREES) * LIFT_GEAR_RATIO;
}

void PYROLift::moveLiftToHeight(float inches, int Velocity){
    //liftMotors.moveAbsolute(TICKS_PER_MOTOR_DEGREE * getMotorDegreesFromLiftDegrees(getAngleForHeight(inches)), Velocity);
    liftTarget = (TICKS_PER_MOTOR_DEGREE * getMotorDegreesFromLiftDegrees(getAngleForHeight(inches)));
}

float PYROLift::getLiftHeight(){
    return (2*(INCHES_PIVOT_TO_PIVOT * (-cos((((liftMotors.getPosition()/TICKS_PER_MOTOR_DEGREE) * (1.0/7.0)) + STARTING_DEGREES) * pi/180.0) + 1)) - HEIGHT_OFFSET);
}

float PYROLift::getLiftAngle(){
    return ((liftMotors.getPosition()/TICKS_PER_MOTOR_DEGREE) * (1.0/LIFT_GEAR_RATIO)) + STARTING_DEGREES;
}

void PYROLift::stackCube(){
    moveLiftToHeight(PICKUP_POSITION, maxVelocityDown);


}
bool doorActivated = false;
bool floorActivated = false;

pros::ADIDigitalOut pistonFloor (7);
pros::ADIDigitalOut pistonDoor (8);

//MotorGroup intake({6,-15});

MotorGroup intake( {Motor(6, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees),
                   Motor(15, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees)});

void PYROLift::manualControl(){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
        intake.tarePosition();
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
        intake.moveAbsolute(-190, 100);
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
        intake.moveVoltage(12000);
    }
    else{
        intake.moveVoltage(0);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        liftTarget += 25;
    }
    else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        liftTarget -= 25;
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        liftMotors.moveVelocity(25);
        liftTarget = liftMotors.getPosition();
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        liftMotors.moveVelocity(-25);
        liftTarget = liftMotors.getPosition();
    }
    else{
        liftMotors.moveAbsolute(liftTarget, 50);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
        doorActivated = !doorActivated;
        pistonDoor.set_value(doorActivated);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
        floorActivated = !floorActivated;
        pistonFloor.set_value(floorActivated);
    }
}
void PYROLift::loopTeleop(){
    if(true){
        manualControl();
    }

}

void PYROLift::tare(){
    liftMotors.moveVelocity(-25);
    pros::delay(500);
    while(abs(liftMotors.getActualVelocity()) > 10){
        pros::delay(10);
    }
    liftMotors.tarePosition();
}

float PYROLift::getMotorPos() {
    return liftMotors.getPosition();
}



float PYROLift::getMotorTemps(){

}