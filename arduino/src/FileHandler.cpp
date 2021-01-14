/**
 * @file FilesHandler.cpp
 * @author yarin
 * @brief 
 * @version 0.1
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "fileHandler.h"
#include <ctime>
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"
#define DEFULT_SENSOR 4

FilesHandler::FilesHandler(int amount = 5)
{
    for (int i = 0; i < amount; i++)
    {
        addFile(makeFileName(i, FILE_NAME, FILE_NAME_END));
    }
}
const std::vector<std::ofstream> &FilesHandler::getFiles() const
{
    return this->files;
}

FilesHandler::ERROR FilesHandler::writeToFile(const std::string &data, int index)
{
    if (index > files.size())
    {
        std::string fileName = makeFileName(index);
        std::ofstream dataLog(fileName, std::ios_base::app);
        if (!dataLog.is_open())
        {
            return FilesHandler::ERROR::FILE_NOT_EXIST;
        }
        this->files.push_back(dataLog);
    }

    files[index] << std::time(nullptr) << std::endl;
    files[index] << data << std::endl;
    files[index].close();
    return FilesHandler::ERROR::SUCCSESS;
}
void FilesHandler::addFile(std::string path)
{
    std::ofstream file;
    file.open(path.c_str(), std::ios_base::app);
    this->files.push_back(file);
}
std::string FilesHandler::makeFileName(int place = DEFULT_SENSOR, std::string startName, std::string endName)
{
    std::string fileName = startName;
    fileName.append(std::to_string(place));
    fileName.append(endName);
    return fileName;
}