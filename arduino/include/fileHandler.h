#ifndef __FILES_HANDLER_H__
#define __FILES_HANDLER_H__

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"
/**
 * @brief handles the log files
 */
class FilesHandler
{
public:
  enum ERROR
  {
    SUCCSESS = 0,
    FILE_ERROR,
    FILE_NOT_EXIST,
  };
  /**
     * @brief Construct a new File Handler object
     */
  FilesHandler(int amount = 5);

  ~FilesHandler();

  /**  
   * @brief return an array of files that the class uses
   * to output of the sensors data.
   * @return std::vector<std::string> an array of string 
   */
  const std::vector<std::ofstream *> &getFiles() const;

  /**
   * @brief dumps data and timestamp to file
   * @param data to write to the file
   * @param index the index of the file from array
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  FilesHandler::ERROR writeToFile(const std::string &data, size_t index);

  /**
   * @brief adds file to the files array
   * @param file the file to add to the array of files
   */
  void addFile(std::string path);

private:
  std::vector<std::ofstream *> files;
};

#endif
