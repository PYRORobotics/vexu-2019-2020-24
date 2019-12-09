/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _ARDUINO_HPP_
#define _ARDUINO_HPP_

#include "api.h"
#include "okapi/api/control/controllerInput.hpp"
#include "okapi/api/device/rotarysensor/continuousRotarySensor.hpp"
#include "okapi/impl/device/rotarysensor/adiGyro.hpp"

// Include sstream for serial parsing
#include <sstream>

// Prototypes for hidden vex functions to bypass PROS bug
extern "C" int32_t vexGenericSerialReceive( uint32_t index, uint8_t *buffer, int32_t length );
extern "C" void vexGenericSerialEnable(  uint32_t index, uint32_t nu );
extern "C" void vexGenericSerialBaudrate(  uint32_t index, uint32_t rate );
extern "C" int32_t vexGenericSerialTransmit( uint32_t index, uint8_t *buffer, int32_t length );


namespace okapi {

class BNO055 : public ADIGyro {
  public:
  /**
   * A gyroscope on the given ADI port. If the port has not previously been configured as a gyro,
   * then the constructor will block for 1 second for calibration. The gyro measures in tenths of a
   * degree, so there are 3600 measurement points per revolution.
   *
   * @param iport the ADI port number
   * @param imultiplier a value multiplied by the gyro heading value
   */
  BNO055(int port);

  virtual ~BNO055();

  /**
   * Get the current sensor value.
   *
   * @return the current sensor value, or ``PROS_ERR`` on a failure.
   */
  double get();

  /**
   * Reset the sensor to zero.
   *
   * @return 1 on success, PROS_ERR on fail
   */
  std::int32_t reset() override;

  /**
   * Get the sensor value for use in a control loop. This method might be automatically called in
   * another thread by the controller.
   *
   * @return the current sensor value, or ``PROS_ERR`` on a failure.
   */
  double controllerGet() override;

  void update(double value);
  void set(double value);
  int get_port();

  protected:
  pros::ADIGyro gyro;
  double value;
  int port;
};


class PYRO_Arduino
{
  public:
    PYRO_Arduino(int port);
    okapi::BNO055 BNO055_Main;
    static okapi::BNO055* bno;
    static void update_differential_pos(void*)
    {
      while(1)
      {
        std::cout << "lolololololol" << std::endl;
        // Start serial on desired port
        vexGenericSerialEnable( bno->get_port() - 1, 0 );

        // Set BAUD rate
        vexGenericSerialBaudrate( bno->get_port() - 1, 115200 );

        // Let VEX OS configure port
        pros::delay(10);



        while (true)
        {


          // Buffer to store serial data
          uint8_t buffer[256];
          int len = 256;

          // Get serial data
          int32_t nRead = vexGenericSerialReceive(bno->get_port() - 1, buffer, len);
          //pros::lcd::print(3, "at while true %d", nRead);

          // Now parse the data
          if (nRead >= 9) {
            std::cout << "lolololololol\n" << std::endl;

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
                      if(heading > 180)
                        heading -= 360;
                      bno->set(heading);
                      std::cout << heading << std::endl;

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
    }
    pros::Task t_arduino_update;
  private:
    int port;
};



} // namespace okapi

//extern okapi::BNO055 BNO055_Main;
extern okapi::PYRO_Arduino Arduino;

#endif
