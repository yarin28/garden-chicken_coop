#include "serialib.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "arduino.h"
#include "fileHandler.h"
#include "utils.h"
#define BANDWITH 9600
#ifdef __linux__
#define SERIAL_PORT "/dev/ttyUSB0"
#endif
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"
#include <spdlog/spdlog.h>

int main(/*int argc, char *argv[]*/)
{
  FilesHandler filesHandler;
  ArduinoClient a(std::string("/dev/ttyUSB0"), BANDWITH, 100000,
                  [&filesHandler](int sensorId, float data) -> int { return filesHandler.writeToFile(std::to_string(data), sensorId); });
  a.openSerial();
  for (size_t i = 0; i < 5; i++)
  {
    filesHandler.addFile(makeFileName(i, FILE_NAME, FILE_NAME_END));
  }

  auto f = [&a]() -> void { a.getDataWithWhileLoop(); };
  f();
  // std::thread *rvc = a.startTheArduinoCheking();
  // rvc->join();
  // delete rvc;
}
