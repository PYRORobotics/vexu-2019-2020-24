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
  public:
    PYRO_Arduino(int port);
    okapi::BNO055 BNO055_Main;
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

        while (true)
        {


          // Buffer to store serial data
          uint8_t buffer[256];
          int len = 256;

          // Get serial data
          int32_t nRead = vexGenericSerialReceive(bno->get_port() - 1, buffer, len);
          //pros::lcd::print(3, "at while true %d", nRead);

          //std::cout << buffer << "|||||";

          //std::string str( buffer, buffer+256 );

          //std::cout << str << "asdfghjkl";


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
            }

            if(*ptr == 2 && *(ptr+1)=='R')
            {

              int checksum = 0;

              while(*ptr!=';')
              {


                checksum += (int) *ptr;

                std::string str(ptr);
                ptr+=sizeof(char);
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


                float heading = stof(s_heading);
                if(heading > 180)
                 heading -= 360;
                bno->set(heading);

                // OrientationData::setAcceleration(x,stof(s_ax));
                // OrientationData::setAcceleration(y,stof(s_ay));
                // OrientationData::setAcceleration(z,stof(s_az));

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
                OrientationData::setAcceleration(x,temp);
                std::stringstream(s_ay) >> temp;
                OrientationData::setAcceleration(y,temp);
                std::stringstream(s_az) >> temp;
                OrientationData::setAcceleration(z,temp);


                // std::cout << ++i << " yay: " << s_heading << " "
                // << s_pitch << " "
                // << s_roll << " "
                // << s_ax << " "
                // << s_ay << " "
                // << s_az << " "
                // << temp
                // << std::endl;

                while(vexGenericSerialReceive(bno->get_port() - 1, buffer, len) > 0){pros::delay(1);}
                pros::delay(80);


// if(i % 20 == 0) reset();






              }
            }
            //
            //
            // char delim[] = " ;";
            //           char* ptr = strtok(input, delim);
            //           while(ptr!=NULL)
            //           {
            //             //std::cout << ptr << std::endl;
            //             std::string str(ptr);
            //
            //
            //             if(str.at(0) == 'R')
            //             {
            //               str.erase(0,1);
            //
            //               char * cstr = new char [str.length()+1];
            //               strcpy (cstr, str.c_str());
            //
            //
            //               char delim[] = ":";
            //               char* ptr1 = strtok(cstr, delim);
            //
            //               int i = 0;
            //
            //               while(ptr1!=NULL)
            //               {
            //                 //std::cout << ptr << std::endl;
            //                 std::string str(ptr);
            //
            //                 if(i == 0)
            //                 {
            //                   double heading = std::stod(str);
            //                   if(heading > 0.000001 || heading == 0)
            //                   {
            //                     if(heading > 180)
            //                       heading -= 360;
            //                     bno->set(heading);
            //                   }
            //                 }
            //
            //                 i++;
            //               }
            //             }
            //
            //
            //
            //
            //
            //
            //
            //     }
          }

            }


          }
          catch(...){}
            pros::delay(20);


      }
    }
    static void reset()
    {
      try
      {
      // Start serial on desired port
      vexGenericSerialEnable( bno->get_port() - 1, 0 );

      // Set BAUD rate
      vexGenericSerialBaudrate( bno->get_port() - 1, 115200 );

      // Let VEX OS configure port
      pros::delay(10);

        // Buffer to store serial data
        char buffer[] = "RESET\n";
        int len = strlen( buffer );



              vexGenericSerialTransmit( bno->get_port() - 1, (uint8_t *)buffer, len  );
        pros::delay(1000);

        }
        catch(...){}
    }

    void setClock();
    pros::Task t_arduino_update;
  private:
    int port;
};



} // namespace okapi

//extern okapi::BNO055 BNO055_Main;
extern okapi::PYRO_Arduino Arduino;

#endif
