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

  okapi::AverageFilter<250> avgFilterReset;

  while(1) // GUI Command
  {
    // std::cout << avgFilterReset.getOutput() << std::endl;
    if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
      avgFilterReset.filter(1);
    else
    {
      for(int i = 0; i < 500; i++)
        avgFilterReset.filter(0);
    }

    if(avgFilterReset.getOutput() == 1)
    {
      PYRO_Arduino::reset();
      for(int i = 0; i < 500; i++)
        avgFilterReset.filter(0);
    }

    if(Screen::DriverEnabled)
    {
      int startTime = pros::millis();

      if(Screen::Screen_Sponsors != NULL)
      lv_obj_set_hidden(Screen::Screen_Sponsors,1);


      bool arcade = false;
      bool voltageControl = false;

      // okapi::ADIEncoder LEFT('A', 'B', true);
      // okapi::ADIEncoder RIGHT('C', 'D');

      // pros::Task lifttaskteleop(liftTask, (void*)NULL, TASK_PRIORITY_DEFAULT,
      //                           TASK_STACK_DEPTH_DEFAULT, "lift teleop task");

      while (Screen::DriverEnabled) // Teleop
      {
        //printf("%f %f\n", LEFT.get(), RIGHT.get());
        chassis.driveController.tank((float) master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127,
                                     (float) -master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) / 127,
                                     0.05);

        if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        {
          // Arduino.setClock();
          chassis.drive_to_coordinate(0, 20, 0);
        }

        if(pros::millis() > 74850 + startTime)
          lv_obj_set_hidden(Screen::Screen_Innovate,0);

        pros::delay(20);
      }

    }
    else if(Screen::AutonEnabled)
    {
      if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        autonomous();
    }

  pros::delay(20);
  }
}

//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
