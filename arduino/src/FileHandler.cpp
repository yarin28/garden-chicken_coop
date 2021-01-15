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
#include "utils.h"
#include <ctime>
#define DEFULT_SENSOR 4
FilesHandler::FilesHandler(int amount)
{ // TODO deleat that shit and move it
    for (int i = 0; i < amount; i++)
    {
        addFile(makeFileName(i, FILE_NAME, FILE_NAME_END));
    }
}

FilesHandler::~FilesHandler()
{
    for (auto file : files)
        delete file;
}
const std::vector<std::ofstream *> &FilesHandler::getFiles() const
{
    return this->files;
}

FilesHandler::ERROR FilesHandler::writeToFile(const std::string &data, size_t index)
{
    if (index > files.size())
    {
        return FilesHandler::ERROR::FILE_ERROR;
    }
    (*files[index]) << std::time(nullptr) << std::endl;
    (*files[index]) << data << std::endl;
    return FilesHandler::ERROR::SUCCSESS;
}
void FilesHandler::addFile(std::string path)
{
    std::ofstream *file = new std::ofstream();
    file->open(path.c_str(), std::ios_base::app);
    this->files.push_back(file);
}
