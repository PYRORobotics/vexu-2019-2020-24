#include "../include/pros/adi.h"
#include "sensors.hpp"


// Include sstream for serial parsing
#include <sstream>

// Prototypes for hidden vex functions to bypass PROS bug
extern "C" int32_t vexGenericSerialReceive( uint32_t index, uint8_t *buffer, int32_t length );
extern "C" void vexGenericSerialEnable(  uint32_t index, uint32_t nu );
extern "C" void vexGenericSerialBaudrate(  uint32_t index, uint32_t rate );
extern "C" int32_t vexGenericSerialTransmit( uint32_t index, uint8_t *buffer, int32_t length );



using namespace okapi;

BNO055::BNO055() : ADIGyro::ADIGyro('G'), gyro('H')
{
  value = 0.0;
}

double BNO055::get()
{
  return value;
}


BNO055::~BNO055() = default;

std::int32_t BNO055::reset() {
  return gyro.reset();
}


double BNO055::controllerGet()
{
  return get();
}

void BNO055::update(double value)
{
  this->value = value;
}


okapi::BNO055 BNO055_Main;


void t_update_BNO055(void*)
{

  // Port to use for serial data
  #define SERIALPORT 16
  // Variable to put the gyro value into

	// Start serial on desired port
	vexGenericSerialEnable( SERIALPORT - 1, 0 );

	// Set BAUD rate
	vexGenericSerialBaudrate( SERIALPORT - 1, 115200 );

	// Let VEX OS configure port
	pros::delay(10);


	while (true)
	{

		// Buffer to store serial data
		uint8_t buffer[256];
		int len = 256;

		// Get serial data
		int32_t nRead = vexGenericSerialReceive(SERIALPORT - 1, buffer, len);

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
                BNO055_Main.update(heading);
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
