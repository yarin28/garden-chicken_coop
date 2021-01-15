/**
 * @file utils.cpp
 * @author yarin
 * @brief utilites
 * @version 0.1
 * @date 15.1.2021
 * @copyright Copyright (c) 2021
 * 
 */
#include "utils.h"
std::string makeFileName(int place, std::string startName, std::string endName)
{
    std::string fileName = startName;
    fileName.append(std::to_string(place));
    fileName.append(endName);
    return fileName;
}
