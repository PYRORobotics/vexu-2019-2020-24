#ifndef _PISTON_H_
#define _PISTON_H_

#include "api.h"

class Piston
{
private:
  int port;
  pros::ADIDigitalOut adi;
  int8_t state;

public:
  Piston(char, int8_t = 0);
  void extend();
  void retract();
  void set(bool);
  void toggle();
};

#endif
