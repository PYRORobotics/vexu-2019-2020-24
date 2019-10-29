#include "main.h"

Piston::Piston(char port, int8_t starting_state): adi(port)
{
  this->port = port;
  state = starting_state;
}

void Piston::extend()
{
  adi.set_value(1);
  state = 1;
}

void Piston::retract()
{
  adi.set_value(0);
  state = 0;
}

void Piston::set(bool output)
{
  adi.set_value(output);
  state = output;
}

void Piston::toggle()
{
  if(state == 1)
  {
    adi.set_value(0);
    state = 0;
  }
  else
  {
    adi.set_value(1);
    state = 1;
  }
}
