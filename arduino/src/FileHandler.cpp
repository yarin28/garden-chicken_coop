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

FilesHandler::FilesHandler(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        addFile(makeFileName(i,FILE_NAME,FILE_NAME_END));
    }
}
FilesHandler::FilesHandler()
{
    for (int i = 0; i < 5; i++)
    {
        addFile(makeFileName(i,FILE_NAME,FILE_NAME_END));
    }
}

const std::vector<std::string> &FilesHandler::getFiles()
{
    return this->files;
}

FilesHandler::ERROR FilesHandler::writeFromBufferToFile(const std::string &data, int place)
{
    std::ofstream dataLog;
    std::string fileName = makeFileName(place);
    dataLog.open(fileName, std::ios_base::app);
    if (!dataLog.is_open())
    {
        return FilesHandler::ERROR::FILE_NOT_EXIST;
    }

    dataLog << std::time(nullptr) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    return FilesHandler::ERROR::SUCCSESS;
}
void FilesHandler::addFile(std::string file)
{
    this->files.push_back(file);
}
std::string FilesHandler::makeFileName( int place = 4,std::string startName , std::string endName )
{
    std::string fileName = startName;
    fileName.append(std::to_string(place));
    fileName.append(endName);
    return fileName;
}