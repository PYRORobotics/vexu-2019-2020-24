//
// Created by charles on 1/9/20.
//
#include "main.h"

std::string LCDLinesOld[4];
const int NUM_LINES = 3;

PYROcontrollerLCD::PYROcontrollerLCD(pros::Controller controller){
    this->controller = controller;
    LCDLines[0] = "";
    LCDLines[1] = "";
    LCDLines[2] = "";
    LCDLines[3] = "";
};


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

void PYROcontrollerLCD::setControllerLCD(int line, std::string linedataString){
    LCDLines[line] = linedataString;
}

void PYROcontrollerLCD::controllerLCDLoop(){
    long rumbleTime = 0;
    std::ostringstream stream;
    //while(true){
    if(true){
        //int timeLeft = 105 - ((pros::millis() - opStartTime)/1000);
        int timeLeft = 105;
        if(timeLeft < 100){
            if((pros::millis() - rumbleTime) > 1000){
                //master.rumble(" ."); //I'm pretty sure brandon has code controlling the rumble somewhere else so I don''t want to conflict with it
                rumbleTime = pros::millis();
                pros::delay(52);
            }
        }
        for(int i = 0; i < (NUM_LINES); i++){
            if(LCDLinesOld[i] != LCDLines[i]){
                LCDLinesOld[i] = LCDLines[i]; //Do this early to try to minimize race condition possibility
                //master.clear_line(i);				//we already add space to the end of a line to fill it, so clearing isn't necessary and would just halve our update rate
                //pros::delay(52);
                controller.print(i, 0, prepareStringForLCD(LCDLines[i]).c_str());
                pros::delay(52);
            }
        }
        stream.str("");
        stream << "Battery: " << pros::battery::get_capacity() << "%%";
        setControllerLCD(2, stream.str());
        pros::delay(60);
    }
}