/**
 * @file arduino.cpp
 * @author yarin
 * @brief  the arduino<-> pi protocol arduino object
 * @version 0.1
 * @date 2020-11-21
 * @copyright Copyright (c) 2020
 * 
 */

#include <ctime>
#include "arduino.h"
#define CHECK_BUFFER_SIZE 64
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"

Arduino::Arduino(const std::string &portName, unsigned int buadRate, unsigned int timeout) : portName(portName), buadRate(buadRate), timeout(timeout)
{
}

Arduino::~Arduino()
{
    this->serial.closeDevice();
}

const std::vector<std::string> &Arduino::getFiles()
{
    return this->files;
}

int Arduino::getTimout()
{
    return this->timeout;
}

void Arduino::setTimeout(int timeout)
{
    this->timeout = timeout;
}

Arduino::ERROR_ARDUINO Arduino::checkConnection()
{
    if (!this->isOpen)
        return Arduino::ERROR_ARDUINO::SUCCSESS;
    char errorCode = serial.openDevice(portName.c_str(), buadRate);
    if (errorCode != 1)
    {
        serial.closeDevice();

        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    }

    serial.closeDevice();
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}

void Arduino::checkConnectionToConsole()
{
    if (checkConnection() != Arduino::ERROR_ARDUINO::SUCCSESS)
    {
        std::cout << "there was a problem with the connection" << std::endl;
    }
    else
    {

        std::cout << "the connection is successful!" << std::endl;
    }
}

void Arduino::addFile(const std::string &file)
{
    this->files.push_back(file);
}

int Arduino::checkSum(const char *message)
{
    int sum = 0;
    const char *p = &message[0];
    while (*p != NULL)
    {
        sum += *message;
        ++p;
    }

    return sum;
}

Arduino::ERROR_ARDUINO Arduino::openSerial()
{
    int err = serial.openDevice(this->portName.c_str(), this->buadRate);
    if (err != 1)
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}

Arduino::ERROR_ARDUINO Arduino::serialCommend(const std::string &message)
{
    if (!this->isOpen)
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    serial.writeString(message.c_str());
    // check sum should be added.
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}

Arduino::ERROR_ARDUINO Arduino::writeFromBufferToFile(const std::string &data, int place)
{
    std::ofstream dataLog;
    std::string fileName = makeFileName(place);
    dataLog.open(fileName, std::ios_base::app);
    if (!dataLog.is_open())
    {
        return Arduino::ERROR_ARDUINO::ERROR_WITH_FILES;
    }

    dataLog << std::time(nullptr) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}

Arduino::ERROR_ARDUINO Arduino::getDataWithWhileLoop()
{
    Arduino::ERROR_ARDUINO return_code = Arduino::ERROR_ARDUINO::SUCCSESS;
    return_code = openSerial();
    if (return_code != Arduino::ERROR_ARDUINO::SUCCSESS)
    {
        goto l_cleanup;
    }
    while (true)
    {
        if (serial.available())
        {
            uint32_t command = receiveInt();
            switch (command)
            {
            case Arduino::HEADERS::GET_FLOAT:
                return_code = receiveFloatFromSensor();
                if (return_code != Arduino::ERROR_ARDUINO::SUCCSESS)
                    goto l_cleanup;
                continue;
            case Arduino::HEADERS::ASK_FOR_OVERALL_STATUS:
                return_code = checkStatusFromArduino();
                if (return_code != Arduino::ERROR_ARDUINO::SUCCSESS)
                    goto l_cleanup;
                continue;
            default:
                continue;
            }
        }
    }

l_cleanup:
    return return_code;
}

Arduino::ERROR_ARDUINO Arduino::receiveFloatFromSensor()
{
    Arduino::ERROR_ARDUINO errorCode = Arduino::ERROR_ARDUINO::SUCCSESS;
    int id = receiveInt();
    float f = receiveFloat();
    errorCode = this->writeFromBufferToFile(std::to_string(f), id);
    return errorCode;
}

float Arduino::receiveFloat()
{
    float value = 0;
    int error_code = serial.readBytes(&value, sizeof(value));
    if (error_code == 1)
        return -1;
    return value;
}

void Arduino::addLogFilesForSensors()
{
    addFile("..//dataFromArduino//sensor0.log");
    addFile("..//dataFromArduino//sensor1.log");
    addFile("..//dataFromArduino//sensor2.log");
    addFile("..//dataFromArduino//sensor3.log");
    addFile("..//dataFromArduino//sensor4.log");
    addFile("..//dataFromArduino//sensor5.log");
}

bool Arduino::receiveBoolean()
{
    uint8_t value = 0;
    serial.readBytes(&value, sizeof(value));
    return -1;
}

Arduino::ERROR_ARDUINO Arduino::checkStatusFromArduino()
{
    if (receiveBoolean())
    {
        std::cout << "the Status is great!" << std::endl;
    }
    else
    {
        std::cout << "the Status is bad!" << std::endl;
        return Arduino::ERROR_ARDUINO::BAD_STATUS;
    }

    return Arduino::ERROR_ARDUINO::SUCCSESS;
}

std::thread *Arduino::startTheArduinoCheking()
{
    return new std::thread([=] { getDataWithWhileLoop(); });
}

int Arduino::receiveInt()
{
    uint32_t value = 0;
    serial.readBytes(&value, sizeof(value));
    return value;
}

std::string Arduino::makeFileName(int place)
{

    std::string fileName = FILE_NAME;
    fileName.append(std::to_string(place));
    fileName.append(FILE_NAME_END);
    return fileName;
}
