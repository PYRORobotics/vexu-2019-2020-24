#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:    lift.cpp
// Author:  Charles Jeffries, Brandon Rice
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
    Motor(13, false, AbstractMotor::gearset::red),
    Motor(14, false, AbstractMotor::gearset::red),
    Motor(2, true, AbstractMotor::gearset::red),
    Motor(3, true, AbstractMotor::gearset::red)}
  );
pros::ADIDigitalIn breakbeam('e');
bool doorActivated = false;
bool floorActivated = false;
pros::ADIDigitalOut pistonFloor (7);
pros::ADIDigitalOut pistonDoor (8);



// Class Defintions
// ----------------

//------------------------------------------------------------------------------
// Method: PYROLift() :
// --------------------
// Description:
//     Constructs a PYROLift object (includes magazine and cube hold pistons).
//
// Parameters:
//```
//    int motorTopRight,
//    int motorBottomRight,
//    int motorTopLeft,
//    int motorBottomLeft,
//    int pneumaticFloorPort,
//    int pneumaticDoorPort
//```
// Objects to Initialize:
//```
//    piston_door (Piston),
//    piston_floor (Piston),
//```
//------------------------------------------------------------------------------
PYROLift::PYROLift( int motorTopRight,
                    int motorBottomRight,
                    int motorTopLeft,
                    int motorBottomLeft,
                    int pneumaticFloorPort,
                    int pneumaticDoorPort) : piston_door(8), piston_floor(7)
{
    cubeCount = 0;
}


//------------------------------------------------------------------------------
// Method: getAngleForHeight(float) : float
// ----------------------------------------
// Description:
//     Converts the input lift height to degrees, for use in collectCube().
//
// Parameters:
//```
//    float inches - the vertical measure of the lift's height (inches).
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getAngleForHeight(float inches){
    //return ((asin(((inches - HEIGHT_OFFSET)/2)/INCHES_PIVOT_TO_PIVOT)) * 180/pi)/1 + 45;
    return acos(-((inches + HEIGHT_OFFSET)/(2 * INCHES_PIVOT_TO_PIVOT)) + 1) * 180/pi;
}


//------------------------------------------------------------------------------
// Method: getMotorDegreesFromLiftDegrees(float) : float
// -----------------------------------------------------
// Description:
//     Converts the input lift degrees to motor degrees, for use in collectCube().
//
// Parameters:
//```
//    float inches - the vertical measure of the lift's height (inches).
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getMotorDegreesFromLiftDegrees(float degrees){
    return (degrees-STARTING_DEGREES) * LIFT_GEAR_RATIO;
}


//------------------------------------------------------------------------------
// Method: moveLiftToHeight(float, int) :
// --------------------------------------
// Description:
//     Sets the liftTarget member to a set height for use in external task control.
//
// Parameters:
//```
//    float inches - how high the lift should move to (inches)
//    int Velocity - not currently used
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROLift::moveLiftToHeight(float inches, int Velocity){
    //liftMotors.moveAbsolute(TICKS_PER_MOTOR_DEGREE * getMotorDegreesFromLiftDegrees(getAngleForHeight(inches)), Velocity);
    liftTarget = (TICKS_PER_MOTOR_DEGREE * getMotorDegreesFromLiftDegrees(getAngleForHeight(inches)));
}


//------------------------------------------------------------------------------
// Method: getLiftHeight() : float
// -------------------------------
// Description:
//     Returns the current height of the lift (inches).
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getLiftHeight(){
    return (2*(INCHES_PIVOT_TO_PIVOT * (-cos((((liftMotors.getPosition()/TICKS_PER_MOTOR_DEGREE) * (1.0/7.0)) + STARTING_DEGREES) * pi/180.0) + 1)) - HEIGHT_OFFSET);
}


//------------------------------------------------------------------------------
// Method: getLiftAngle() : float
// ------------------------------
// Description:
//     Returns the current angle of the lift (degrees).
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getLiftAngle(){
    return ((liftMotors.getPosition()/TICKS_PER_MOTOR_DEGREE) * (1.0/LIFT_GEAR_RATIO)) + STARTING_DEGREES;
}


//------------------------------------------------------------------------------
// Method: intakeAndCollect() :
// ----------------------------
// Description:
//     Runs the intake until a cube is in position, then fires the lift down to
//     collect the cube via collectCube().
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
// Method: collectCube() :
// -----------------------
// Description:
//     Runs the lift down to collect a cube, then runs the lift back up to the
//     HOVER_HEIGHT.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROLift::collectCube(){

    liftMotors.moveVelocity(-60);
    pros::delay(250);
    while(abs(liftMotors.getActualVelocity()) > 3){
        pros::delay(10);
    }
    moveLiftToHeight(HOVER_HEIGHT, 50);
    liftTarget = getMotorDegreesFromLiftDegrees(getAngleForHeight(HOVER_HEIGHT));
    cubeCount++;
}


void PYROLift::collectCube(int velocity){

    liftMotors.moveVelocity(-abs(velocity));
    pros::delay(250);
    while(abs(liftMotors.getActualVelocity()) > 3){
        pros::delay(10);
    }
    moveLiftToHeight(HOVER_HEIGHT, 50);
    liftTarget = getMotorDegreesFromLiftDegrees(getAngleForHeight(HOVER_HEIGHT));
    cubeCount++;
}


//------------------------------------------------------------------------------
// Method: manualControl() :
// -------------------------
// Description:
//     Allows the driver to control the lift in opcontrol.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
// Method: manualControl() :
// -------------------------
// Description:
//     The task method containing the logic for controlling the lift during opcontrol.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROLift::loopTeleop(){
    moveLiftToHeight(HOVER_HEIGHT, 50);
    while(true) {
        if (true) {
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
                if(getLiftHeight() > 10) {
                    intakeAndCollect();
                }
                else{
                    moveLiftToHeight(HOVER_HEIGHT, 50);
                }
            }
            manualControl();
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
            tare();
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            moveLiftToHeight(30, 50);
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){
            pistonFloor.set_value(true);
            pros::delay(50);
            pistonFloor.set_value(false);
        }
        printf("Breakbeam: %d\n", breakbeam.get_value());
    }
}


//------------------------------------------------------------------------------
// Method: tare() :
// ----------------
// Description:
//     Zeroes the lift by running the lift downward until its velocity is 0.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    void
//```
//------------------------------------------------------------------------------
void PYROLift::tare(){
    liftMotors.moveVelocity(-25);
    pros::delay(500);
    while(abs(liftMotors.getActualVelocity()) > 10){
        pros::delay(10);
    }
    liftMotors.tarePosition();
}


//------------------------------------------------------------------------------
// Method: getMotorPos() : float
// -----------------------------
// Description:
//     Returns the liftMotors encoder position.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getMotorPos() {
    return liftMotors.getPosition();
}


//------------------------------------------------------------------------------
// Method: getMotorTemps() : float
// -------------------------------
// Description:
//     Returns the liftMotors temperature.
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    float
//```
//------------------------------------------------------------------------------
float PYROLift::getMotorTemps(){
    return liftMotors.getTemperature();
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
