#include "../include/pros/adi.h"
#include "bno055.hpp"

//==================================START FILE==================================
//==============================================================================
// File:		bno055.cpp
// Author:	Brandon Rice
// Created: 4 August 2019
//
// Description:
// ------------
// This file contains the definitions of the BNO055 class (inherited from
// okapi::ADIGyro).
//
//------------------------------------------------------------------------------



// File Setup
// ----------

using namespace okapi;


// Global Objects Created in bno055.hpp
// ------------------------------------

okapi::BNO055 BNO055_Main(16);


// Function Defintions
// -------------------


//------------------------------------------------------------------------------
// Function: t_update_BNO055() : void
// ----------------------------
// Description:
// 		This function is used in the creation of a PROS Task (pros::Task) to read
//    BNO055's data from a Smart Cable's serial port and set the BNO055 object's
//    data attributes accordingly.
//
// Parameters:
//```
//		void* port (the port number which the offboard microcontroller controlling
//    the BNO055 is plugged into)
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void t_update_BNO055(void*)
{

	// Start serial on desired port
	vexGenericSerialEnable( BNO055_Main.get_port() - 1, 0 );

	// Set BAUD rate
	vexGenericSerialBaudrate( BNO055_Main.get_port() - 1, 115200 );

	// Let VEX OS configure port
	pros::delay(10);


	while (true)
	{

		// Buffer to store serial data
		uint8_t buffer[256];
		int len = 256;

		// Get serial data
		int32_t nRead = vexGenericSerialReceive(BNO055_Main.get_port() - 1, buffer, len);

		// Now parse the data
		if (nRead >= 9) {

				// Stream to put the characters in
				std::stringstream myStream("");
				bool recordAngle = false;

				// Go through characters
				for (int i = 0; i < nRead; i++) {
						// Get current char
						char thisDigit = (char)buffer[i];

						// If its special, then don't record the value
						if (thisDigit == 'D' || thisDigit == 'I' || thisDigit == 'A')
								recordAngle = false;

						// Finished recieving angle, so put into variable
						if (thisDigit == 'E') {
								recordAngle = false;
                double heading;
								myStream >> heading;
								heading -= 180;
                BNO055_Main.set(heading);
						}

						// If we want the digits, put them into stream
						if (recordAngle)
								myStream << (char)buffer[i];

						// If the digit is 'A', then the following data is the angle
						if (thisDigit == 'A')
								recordAngle = true;

				}
			}
			pros::delay(20);
		}
}


// Class Defintions
// ----------------


//------------------------------------------------------------------------------
// Method: BNO055() :
// ----------------------------
// Description:
// 		Sets up the BNO055 as a okapi::ADIGyro "using" ports 'G' and 'H'. Neither
//    of these ports are really used, they just need to be valid for the
//    v5 to set up gyro objects without errors.
//
// Parameters:
//```
//		port (the v5 port number that the BNO055 is plugged into)
//```
// Objects to Initialize:
//```
//		ADIGyro::ADIGyro (unused), gyro (unused)
//```
//------------------------------------------------------------------------------
BNO055::BNO055(int port) : ADIGyro::ADIGyro('G'), gyro('H')
{
  this->port = port;
  value = 0.0;
}


//------------------------------------------------------------------------------
// Method: get() : double
// ----------------------------
// Description:
// 		Returns the current (last read) value of the BNO055.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		value (current value of BNO055)
//```
//------------------------------------------------------------------------------
double BNO055::get()
{
  return value;
}


//------------------------------------------------------------------------------
// Method: ~BNO055() :
// ----------------------------
// Description:
// 		Default destructor.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
BNO055::~BNO055() = default;


//------------------------------------------------------------------------------
// Method: reset() : std::int32_t
// ----------------------------
// Description:
// 		NOT FINISHED! Resets (zeros) the BNO055's readings.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		The status of the reset.
//```
//------------------------------------------------------------------------------
std::int32_t BNO055::reset() {
  return gyro.reset();
}


//------------------------------------------------------------------------------
// Method: controllerGet() : double
// ----------------------------
// Description:
// 		Duplicate of get(); exists for inheritance purposes. Use get() instead!
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		get()
//```
//------------------------------------------------------------------------------
double BNO055::controllerGet()
{
  return get();
}


//------------------------------------------------------------------------------
// Method: set() : void
// ----------------------------
// Description:
// 		Method used to set the value of the BNO055, used in t_update_BNO055 to
//    update value from the serial stream.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		None
//```
//------------------------------------------------------------------------------
void BNO055::set(double value)
{
  this->value = value;
}


//------------------------------------------------------------------------------
// Method: get_port() : int
// ----------------------------
// Description:
// 		Returns the port of the BNO055.
//
// Parameters:
//```
//		None
//```
// Returns:
//```
//		value (current value of BNO055)
//```
//------------------------------------------------------------------------------
int BNO055::get_port()
{
  return port;
}



//------------------------------------------------------------------------------
//===================================END FILE===================================
//==============================================================================
