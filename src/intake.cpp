#include "intake.h"

using namespace okapi;

PYROIntake::PYROIntake() : motors({Motor(6, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees),
                    Motor(15, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees)})
{
}

PYROIntake intake;
