/**
 * @file FileHandler.cpp
 * @author yarin
 * @brief 
 * @version 0.1
 * @date 2021-01-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "fileHandler.h"
#include <ctime>
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"

FileHandler::FileHandler()
{
    for (int i = 0; i < 5; i++)
    {
        addFile(makeFileName(i));
    }
}

const std::vector<std::string> &FileHandler::getFiles()
{
    return this->files;
}
FileHandler::ERROR FileHandler::writeFromBufferToFile(const std::string &data, int place)
{
    std::ofstream dataLog;
    std::string fileName = makeFileName(place);
    dataLog.open(fileName, std::ios_base::app);
    if (!dataLog.is_open())
    {
        return FileHandler::ERROR::FILE_UNAVALBLE;
    }

    dataLog << std::time(nullptr) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    // spdlog::get("ylogger")->info("one was printed");
    return FileHandler::ERROR::SUCCSESS;
}
void FileHandler::addFile(std::string file)
{
    this->files.push_back(file);
}
std::string FileHandler::makeFileName(int place)
{
    std::string fileName = FILE_NAME;
    fileName.append(std::to_string(place));
    fileName.append(FILE_NAME_END);
    return fileName;
}