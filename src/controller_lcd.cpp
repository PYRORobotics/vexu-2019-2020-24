//
// Created by charles on 1/9/20.
//
#include "main.h"

//Strings that hold the previous cycle's text data for use for comparisons this cycle.
std::string LCDLinesOld[4];
const int NUM_LINES = 3;

PYROcontrollerLCD::PYROcontrollerLCD(pros::Controller acontroller) : controller(pros::E_CONTROLLER_PARTNER) {   //controller need to be explicitly initialized, so we just set it to the partner joystick. This isn't an issue since it gets set to whatever controller object was passed through
    controller = acontroller;
    LCDLines[0] = "";
    LCDLines[1] = "";
    LCDLines[2] = "";
    LCDLines[3] = "";
};

//------------------------------------------------------------------------------
// Function: prepareStringForLCD(std::string input) :
// ---------------------------
// Description:
//     Prepares a passed string for display on the controller LCD. To do this it
//     makes the string 15 characters long. If the string is too long, it deletes
//     excess characters; if it is too short, it adds spaces tot he end to ensure
//     that the line on the LCD is completely overwritten, removing the need for
//     a clear() call, doubling the update rate of the display.
//
// Parameters:
//```
//    std::string input
//```
// Returns:
//```
//    prepared string
//```
//------------------------------------------------------------------------------
std::string PYROcontrollerLCD::prepareStringForLCD(std::string input){
    if(input.size() <= 15){
        input.insert(input.end(), 15-input.size(), ' ');
    }
    else{
        input.erase(input.begin() + 15, input.end());
    }
    std::string output = input;
    return output;
}

//------------------------------------------------------------------------------
// Function: setControllerLCD(int line, std::string linedataString) :
// ---------------------------
// Description:
//     Setter for writing to the LCDLines array.
//
// Parameters:
//```
//    int line
//    std::string linedataString
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void PYROcontrollerLCD::setControllerLCD(int line, std::string linedataString){
    LCDLines[line] = linedataString;
}

//------------------------------------------------------------------------------
// Function: controllerLCDLoop() :
// ---------------------------
// Description:
//     Function containing the main code for async control of the controller LCD
//
// Parameters:
//```
//    None
//```
// Returns:
//```
//    None
//```
//------------------------------------------------------------------------------
void PYROcontrollerLCD::controllerLCDLoop(){
    for(int i = 0; i < (NUM_LINES); i++){   //loop through all lines
        if(LCDLinesOld[i] != LCDLines[i]){  //if something has been changed, we need to update that line of the display. Otherwise, do nothing because writes to the controller LCD are costly in terms of time.
            LCDLinesOld[i] = LCDLines[i]; //Do this early to try to minimize race condition possibility
            controller.print(i, 0, prepareStringForLCD(LCDLines[i]).c_str());
            pros::delay(52);    //since we wrote to the display, we need to wait the 50ms (52ms because I don't trust vex fully) cooldown before we attempt to write again
        }
    }
    //pros::delay(50);    //don't hog the CPU   //there is a delay in the loop that calls this function, so this delay is no longer necessary.
}