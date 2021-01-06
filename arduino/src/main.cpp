#include "serialib.h"
#include <stdio.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "arduino.h"
#define BANDWITH 9600
#ifdef __linux__
#define SERIAL_PORT "/dev/ttyUSB0"
#endif
#include <spdlog/spdlog.h>

int main(/*int argc, char *argv[]*/)
{

  auto logger = spdlog::basic_logger_mt("ylogger", "../logs/arduinoClient.log");
  spdlog::set_default_logger(logger);
  spdlog::flush_on(spdlog::level::info);
  spdlog::get("ylogger")->info("my name is yarin");
  ArduinoClient a("/dev/ttyUSB0", BANDWITH, 100000);
  a.openSerial();
  std::cout << "poopo" << std::endl;
  std::thread *rvc = a.startTheArduinoCheking();
  rvc->join();
  delete rvc;
}
