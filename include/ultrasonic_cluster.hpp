/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _ULTRASONIC_CLUSTER_HPP_
#define _ULTRASONIC_CLUSTER_HPP_

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


extern pros::ADIUltrasonic Ultrasonic_Front;
extern pros::ADIUltrasonic Ultrasonic_Right;
extern pros::ADIUltrasonic Ultrasonic_Rear;
extern pros::ADIUltrasonic Ultrasonic_Left;

#endif
