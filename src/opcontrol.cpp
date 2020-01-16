#include "main.h"

//==================================START FILE==================================
//==============================================================================
// File:    opcontrol.cpp
// Author:  Brandon Rice, Charles Jeffries
// Created: 13 July 2019
// Last Modified: 29 October 2019
//
// Description:
// ------------
// From PROS:
// * Runs the operator control code. This function will be started in its own task
// * with the default priority and stack size whenever the robot is enabled via
// * the Field Management System or the VEX Competition Switch in the operator
// * control mode.
// *
// * If no competition control is connected, this function will run immediately
// * following initialize().
// *
// * If the robot is disabled or communications is lost, the
// * operator control task will be stopped. Re-enabling the robot will restart the
// * task, not resume it from where it left off.
//
//------------------------------------------------------------------------------


// Global/Static Objects
// ---------------------
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);
PYROcontrollerLCD masterLCD(master);
PYROcontrollerLCD partnerLCD(partner);



// Function Defintions
// -------------------

//------------------------------------------------------------------------------
// Function: liftTask(void*) :
// ---------------------------
// Description:
//     Runs the teleop functions for the lift and intake (for use with pros::Task).
//
// Parameters:
//```
//    void* parameters
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void liftTask(void*)
{
    lift.loopTeleop();
}

//------------------------------------------------------------------------------
// Function: controllerLCDTask(void*) :
// ---------------------------
// Description:
//     Runs the Async controller LCD functions (for use with pros::Task).
//
// Parameters:
//```
//    void* parameters
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void controllerLCDTask(void*){
    while(true) {
        masterLCD.controllerLCDLoop();
        //partnerLCD.controllerLCDLoop(); //If the partner joystick isn't actually being used, then there's no point in halving our screen update frequency.
    }
}

//------------------------------------------------------------------------------
// Function: printSampleControllerText() :
// -----------------------
// Description:
//     Function that displays some sample values to the master controller's screen
// Parameters:
//```
//    None
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void printSampleControllerText(){
    std::ostringstream stream;
    stream.str("");
    stream << "L_Stick: " << master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    masterLCD.setControllerLCD(0, stream.str());
    stream.str("");
    stream << "R_Stick: " << master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    masterLCD.setControllerLCD(1, stream.str());
    stream.str("");
    stream << "Battery: " << pros::battery::get_capacity() << "%%";
    masterLCD.setControllerLCD(2, stream.str());
}


//------------------------------------------------------------------------------
// Function: opcontrol() :
// -----------------------
// Description:
//     The function (task) that will be scheduled after initialize(), and after
//     competition_initialize() and autonomous() if connected to a competition
//     switch. This function will loop indefinately at roughly 50 Hz until the
//     user stops the program.
// Parameters:
//```
//    None
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void opcontrol() {

    bool arcade = false;
    bool voltageControl = false;

    okapi::ADIEncoder LEFT('A', 'B', true);
    okapi::ADIEncoder RIGHT('C', 'D');

    pros::Task lifttaskteleop(liftTask, (void*)NULL, TASK_PRIORITY_DEFAULT,
                            TASK_STACK_DEPTH_DEFAULT, "lift teleop task");
    pros::Task taskControllerLCD(controllerLCDTask, (void*)NULL, TASK_PRIORITY_DEFAULT,
                              TASK_STACK_DEPTH_DEFAULT, "controller LCD task");
    while (true)
    {
        printf("%f %f\n", LEFT.get(), RIGHT.get());
        chassis.driveController.tank((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                     (float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127,
                                     0.05);
        printSampleControllerText();
        pros::delay(20);
    }
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
