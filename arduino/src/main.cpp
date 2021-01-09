#include "serialib.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "arduino.h"
#include "fileHandler.h"
#define BANDWITH 9600
#ifdef __linux__
#define SERIAL_PORT "/dev/ttyUSB0"
#endif
#include <spdlog/spdlog.h>

int main(/*int argc, char *argv[]*/)
{
  FilesHandler filesHandler;
  ArduinoClient a(std::string("/dev/ttyUSB0"), BANDWITH, 100000,
                  [&filesHandler](int sensorId, float data) -> int { return filesHandler.writeFromBufferToFile(std::to_string(data), sensorId); });
  a.openSerial();
  std::thread *rvc = a.startTheArduinoCheking();
  rvc->join();
  delete rvc;
}
