//
// Created by charles on 1/9/20.
//

#ifndef VEXU_2019_2020_24_CONTROLLER_LCD_H
#define VEXU_2019_2020_24_CONTROLLER_LCD_H

class PYROcontrollerLCD {
    public:
        PYROcontrollerLCD(pros::Controller controller);
        void controllerLCDLoop();
        void setControllerLCD(int line, std::string linedataString);
    private:
        std::string prepareStringForLCD(std::string input);
        std::string LCDLines[4];
        pros::Controller controller;
};

extern PYROcontrollerLCD masterLCD;
extern PYROcontrollerLCD partnerLCD;

#endif //VEXU_2019_2020_24_CONTROLLER_LCD_H
