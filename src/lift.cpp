#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:		chassis.cpp
// Author:	Charles Jeffries, Brandon Rice
// Created: 7 October 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// This file contains the definitions of the PYROLift class.
//
//------------------------------------------------------------------------------


// Global/Static Objects
// ---------------------



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
const float HOVER_HEIGHT = 11.0;
const int ULTRASONIC_THRESHOLD = 70;
int PYROLift::liftTarget = 10;
PYROLift lift(11,12,1,2,5,6);

okapi::MotorGroup PYROLift::liftMotors({
    Motor(11, false, AbstractMotor::gearset::red),
    Motor(13, false, AbstractMotor::gearset::red),
    Motor(3, true, AbstractMotor::gearset::red),
    Motor(2, true, AbstractMotor::gearset::red)}
  );


pros::ADIDigitalIn breakbeam('e');

float degreesToRadians(float degrees){

}

PYROLift::PYROLift(
        int motorTopRight,
        int motorBottomRight,
        int motorTopLeft,
        int motorBottomLeft,
        int pneumaticFloorPort,
        int pneumaticDoorPort) :
        piston_door(8),
        piston_floor(7){

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

void PYROLift::intakeAndCollect(){
    intake.motors.moveVoltage(12000);
    while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && breakbeam.get_value()){
        printf("breakbeam: %d\n", breakbeam.get_value());
        pros::delay(10);
    }
    if(!breakbeam.get_value()) {
        intake.motors.moveVoltage(5000);
        collectCube();
    }
    intake.motors.moveVoltage(0);
}

void PYROLift::collectCube(){

    liftMotors.moveVelocity(-50);
    pros::delay(250);
    while(abs(liftMotors.getActualVelocity()) > 10){
        pros::delay(10);
    }
    moveLiftToHeight(HOVER_HEIGHT, 50);
    liftTarget = getMotorDegreesFromLiftDegrees(getAngleForHeight(HOVER_HEIGHT));
    cubeCount++;
}
bool doorActivated = false;
bool floorActivated = false;

pros::ADIDigitalOut pistonFloor (7);
pros::ADIDigitalOut pistonDoor (8);

//MotorGroup intake({6,-15});


void PYROLift::manualControl(){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
        intake.motors.tarePosition();
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intake.motors.moveAbsolute(-190, 100);
    }

    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
        intake.motors.moveVoltage(12000);
    }
    else{
        intake.motors.moveVoltage(0);
    }


    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        liftTarget += 25;
    }
    else if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
        liftTarget -= 25;
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        liftMotors.moveVelocity(50);
        liftTarget = liftMotors.getPosition();
    }
    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        liftMotors.moveVelocity(-50);
        liftTarget = liftMotors.getPosition();
    }
    else{
        liftMotors.moveAbsolute(liftTarget, 50);
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)){
        //doorActivated = !doorActivated;
        pistonDoor.set_value(true);
    }
    else{
        pistonDoor.set_value(false);
    }

    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
        //floorActivated = !floorActivated;
        pistonFloor.set_value(true);
    }
    else{
        pistonFloor.set_value(false);
    }
}
void PYROLift::loopTeleop(){
    while(true) {
        if (true) {
            if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
                intakeAndCollect();
            }
            manualControl();
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            tare();
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            moveLiftToHeight(30, 50);
        }
        printf("Breakbeam: %d\n", breakbeam.get_value());
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
    return liftMotors.getTemperature();
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
