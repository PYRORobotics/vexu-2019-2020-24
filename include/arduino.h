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
  private:
    int port;
};


} // namespace okapi

//extern okapi::BNO055 BNO055_Main;
extern okapi::PYRO_Arduino Arduino;

#endif
