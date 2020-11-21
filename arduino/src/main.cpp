/**
 * @file /example1/main.cpp
 * @author Philippe Lucidarme
 * @date December 2019
 * @brief File containing example of serial port communication
 *
 * This example send the ASCII table through the serial device
 *
 * @see https://lucidar.me
 */

// Serial library
#include "serialib.h"
#include <stdio.h>
#include <iostream>
#include "arduino.h"
#define BANDWITH 9600
#ifdef __linux__
#define SERIAL_PORT "/dev/ttyUSB0"
#endif

/*!
 * \brief main  Simple example that send ASCII characters to the serial device
 * \return      0 : success
 *              <0 : an error occured
 */

int main(/*int argc, char *argv[]*/)
{

  Arduino a;
  a.openSerial();
  //a.serialCommend("201");
  a.addFile("../dataFromArduino/sensor1.log");
  a.writeFromBufferToFile("hello nigi",0);
  return 0;
}

// int main(/*int argc, char *argv[]*/)
// {

//   serialib serial;
//   // Arduino a =("outputfile.txt","/dev/ttyUSB0",9600,10000);
//   // Serial object
//   Arduino a;

//   // Connection to serial port
//   char errorOpening = serial.openDevice(SERIAL_PORT, BANDWITH);
//   a.checkConnectionToConsule();
//   a.checkSum("abc");
//   // If connection fails, return the error code otherwise, display a success message
//   if (errorOpening != 1)
//     return errorOpening;
//   std::cout << "Successful connection to " << SERIAL_PORT << std::endl;

//   char str[64] = {0};
//   int err = serial.readString(str, '!', 64);
//   std::cout << str << std::endl
//             << "err: " << err << std::endl;

//   // Close the serial device
//   serial.closeDevice();

//   return 0;
// }
