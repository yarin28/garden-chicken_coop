#include "serialib.h"
#include <stdio.h>
#include <iostream>
#include "arduino.h"
#define BANDWITH 9600
#ifdef __linux__
#define SERIAL_PORT "/dev/ttyUSB0"
#endif

int main(/*int argc, char *argv[]*/)
{

  Arduino a("/dev/ttyUSB0", BANDWITH, 100000);
  a.openSerial();
  a.addLogFilesForSensors();
  std::thread *rvc = a.startTheArduinoCheking();
  rvc->join();
  delete rvc;
}
