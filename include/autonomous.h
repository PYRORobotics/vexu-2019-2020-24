#ifndef _AUTONOMOUS_H_
#define _AUTONOMOUS_H_


enum autonNames
{
  red0,
  blue0,
  skills0
};


typedef autonNames autonID;

static autonID selectedAutonID;


extern void autonomousSelector(autonID);
extern void auton_red0();
extern void auton_blue0();
extern void auton_skills0();

#endif
