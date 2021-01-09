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
  FilesHandler();
  /**
   * @brief return an array of files that the class uses
   * to output of the sensors data.
   * @return std::vector<std::string> an array of string 
   */
  FilesHandler(int amount);

  const std::vector<std::string> &getFiles();

  /**
   * @brief dumps data and timestamp to file
   * @param data to write to the file
   * @param place the index of the file from array
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  FilesHandler::ERROR writeFromBufferToFile(const std::string &data, int place);

  /**
   * @brief adds file to the files array
   * @param file the file to add to the array of files
   */
  void addFile(std::string file);

  /**
   * @brief make file name form the place.
   * @param the id of the sensor
    @return the file name to open.
  */

std::string makeFileName( int place,std::string startName = FILE_NAME, std::string endName = FILE_NAME_END);

private:
  std::vector<std::string> files;
};