#ifndef __UTILS_H__
#define __UTILS_H__
#include <iostream>
#include <string>
/**
   * @brief make file name form the place.
   * @param the id of the sensor
    @return the file name to open.
  */
std::string makeFileName(int place, std::string startName, std::string endName);

#endif
