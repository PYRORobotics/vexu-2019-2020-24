/**
 * @author Ryan Benasutti, WPI
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include "api.h"
#include "okapi/api/control/controllerInput.hpp"
#include "okapi/api/device/rotarysensor/continuousRotarySensor.hpp"
#include "okapi/impl/device/rotarysensor/adiGyro.hpp"
#include "okapi/api/filter/averageFilter.hpp"
#include "okapi/api/filter/medianFilter.hpp"

// Include sstream for serial parsing
#include <sstream>

#include "localization.h"

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
  private:
    inline static int resetTimeout = 5000;
  public:
    PYRO_Arduino(int port);
    okapi::BNO055 BNO055_Main;
    inline static bool isCommumnicating = false;
    static okapi::BNO055* bno;
    static void update_differential_pos(void*)
    {
      while(1)
      {
        try
        {

        // Start serial on desired port
        vexGenericSerialEnable( bno->get_port() - 1, 0 );

        // Set BAUD rate
        vexGenericSerialBaudrate( bno->get_port() - 1, 115200 );

        // Let VEX OS configure port
        pros::delay(10);

        int i = 0;
        int j = 0;

        okapi::AverageFilter<5> avgFilterComms;
        okapi::MedianFilter<3> medFilterAx;
        okapi::MedianFilter<3> medFilterAy;
        okapi::MedianFilter<3> medFilterAz;
        okapi::MedianFilter<3> medFilterHead;

        while (true)
        {
          isCommumnicating = 0;


          // Buffer to store serial data
          uint8_t buffer[256];
          int len = 256;

          // Get serial data
          int32_t nRead = vexGenericSerialReceive(bno->get_port() - 1, buffer, len);
          //pros::lcd::print(3, "at while true %d", nRead);

          //std::cout << buffer << "|||||";

          //std::string str( buffer, buffer+256 );

          //std::cout << str << "asdfghjkl";


          if(avgFilterComms.filter(nRead) > 0)
            isCommumnicating = 1;


          char* input = (char*) buffer;

          char* ptr = input;

          if(ptr != NULL)
          {
            //if(input[0] != 2)
            //std::cout << ++j << "oops\n";


            int count = 0;
            while((*ptr != 2 || *ptr == '\0') && count < 255)
            {
              ptr+=sizeof(char);
              count++;
              // pros::delay(1);
            }

            if(*ptr == 2 && *(ptr+1)=='R')
            {

              int checksum = 0;

              while(*ptr!=';' && count<60)
              {


                checksum += (int) *ptr;

                std::string str(ptr);
                ptr+=sizeof(char);
                // pros::delay(1);
              }

              checksum += (int) *ptr;
              ptr+=sizeof(char);

              char key = *(ptr);


              checksum = checksum % 128 + 3;

              //std::cout << input << "\n";
              //std::cout << (char)checksum << " = " << key<<std::endl;

              if((char)checksum == (char)key)
              {

                //ptr-=58*sizeof(char);
                ptr = input;// + 2*sizeof(char);

                char c_str[60] = {0};
                for(int i = 0; i < 60; i++)
                {
                  c_str[i] = *ptr;
                  ptr+=sizeof(char);
                }
                //ptr+=sizeof(char);

                std::string s_str(c_str);

                std::string s_heading = s_str.substr(2,7);
                std::string s_pitch = s_str.substr(10,7);
                std::string s_roll = s_str.substr(18,7);
                std::string s_ax = s_str.substr(26,7);
                std::string s_ay = s_str.substr(34,7);
                std::string s_az = s_str.substr(42,7);

                std::string s_time_hex = s_str.substr(50,8);

                int time;
                std::istringstream(s_time_hex) >> std::hex >> time;
                // std::cout << s_time_hex << " = " << time << std::endl;


                float heading = stof(s_heading);
                if(heading > 180)
                 heading -= 360;

                 bool gotMutex = false;

                gotMutex = OrientationData::mutex.take(500);
                if(gotMutex)
                {
                  bno->set(medFilterHead.filter(heading));

                  float temp;

                  size_t position = s_ax.find("-");
                  if(position!=std::string::npos)
                    s_ax = s_ax.substr(position);     // get from position to the end

                  position = s_ay.find("-");
                  if(position!=std::string::npos)
                    s_ay = s_ay.substr(position);     // get from position to the end

                  position = s_az.find("-");
                  if(position!=std::string::npos)
                    s_az = s_az.substr(position);     // get from position to the end


                  std::stringstream(s_ax) >> temp;
                  OrientationData::setAcceleration(x,medFilterAx.filter(temp));
                  std::stringstream(s_ay) >> temp;
                  OrientationData::setAcceleration(y,medFilterAy.filter(temp));
                  std::stringstream(s_az) >> temp;
                  OrientationData::setAcceleration(z,medFilterAz.filter(temp));

                  OrientationData::setTime(time);

                  OrientationData::mutex.give();

                }


                buffer[0]='\0';
                while(vexGenericSerialReceive(bno->get_port() - 1, buffer, len) > 0){
                  buffer[0]='\0';
                  pros::delay(1);
                }
                pros::delay(20);






              }
            }


          }
          if(pros::millis() > resetTimeout || OrientationData::isOld())
          {
            resetTimeout += 10000;
            setClock();
          }

              pros::delay(20);
            }


          }
          catch(...){
            isCommumnicating = 0;
          }
            pros::delay(20);


      }

    }
    static void reset()
    {
      try
      {
        std::cout << "Trying to send RESET Arduino" << std::endl;
      // // Start serial on desired port
      // vexGenericSerialEnable( bno->get_port() - 1, 0 );
      //
      // // Set BAUD rate
      // vexGenericSerialBaudrate( bno->get_port() - 1, 115200 );


      FILE* port7 = fopen("/dev/7", "w");


      // Let VEX OS configure port
      pros::delay(10);

        // Buffer to store serial data
        // uint8_t buffer[6] = "C";
        // int len = 6;


        std::string str = "RESET";
        const char *cstr = str.c_str();


        fputs(cstr, port7);
        fclose(port7);


        // // Get serial data
        // vexGenericSerialTransmit(bno->get_port() - 1, buffer, len);


        pros::delay(10);
        std::cout << "Successfully RESET Arduino" << std::endl;


        }
        catch(...)
        {
          std::cout << "ERROR ON RESET Arduino!!!" << std::endl;
        }
    }

    static void setClock()
    {
      try
      {
        std::cout << "Trying to send SetClock" << std::endl;
      // // Start serial on desired port
      // vexGenericSerialEnable( bno->get_port() - 1, 0 );
      //
      // // Set BAUD rate
      // vexGenericSerialBaudrate( bno->get_port() - 1, 115200 );


      FILE* port7 = fopen("/dev/7", "w");


      // Let VEX OS configure port
      pros::delay(10);

        // Buffer to store serial data
        // uint8_t buffer[6] = "C";
        // int len = 6;


        std::string str = "CLOCK";
        str += std::to_string(pros::millis());
        const char *cstr = str.c_str();


        fputs(cstr, port7);
        fclose(port7);


        // // Get serial data
        // vexGenericSerialTransmit(bno->get_port() - 1, buffer, len);


        pros::delay(10);
        std::cout << "Successfully SetClock" << std::endl;


        }
        catch(...)
        {
          std::cout << "ERROR ON SETCLOCK!!!" << std::endl;
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
