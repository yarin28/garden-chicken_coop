#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
/**
 * @brief handles the log files
 */
class FileHandler
{
public:
  enum ERROR
  {
    SUCCSESS = 0,
    FILE_UNAVALBLE,
    FILE_DOES_NOT_EXIST,
  };
  /**
     * @brief Construct a new File Handler object
     */
  FileHandler();
  /**
   * @brief return an array of files that the class uses
   * to output of the sensors data.
   * @return std::vector<std::string> an array of string 
   */
  const std::vector<std::string> &getFiles();

  /**
   * @brief dumps data and timestamp to file
   * @param data to write to the file
   * @param place the index of the file from array
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  FileHandler::ERROR writeFromBufferToFile(const std::string &data, int place);

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
  std::string makeFileName(int place);

  std::vector<std::string> files;
};