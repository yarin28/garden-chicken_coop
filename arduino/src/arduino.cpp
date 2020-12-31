/**
 * @file arduino.cpp
 * @author yarin
 * @brief  the arduino<-> pi protocol arduino object
 * @version 0.1
 * @date 2020-11-21
 * @copyright Copyright (c) 2020
 * 
 */

#include "arduino.h"
#define RATE 9600
#define TIMEOUT 10000
#define READLENGTH 100
#define SUCCESS 1
#define DATALEN 1000
#define COMMENDHEADER "3"
#define FAILURE -1
#define NULLCHAR '\0'
#define CHECKBUFFSIZE 64

/*!
 * @brief Destroy the Arduino:: Arduino object 
 * probably meaningless 

 */
Arduino::~Arduino()
{
    this->serial.closeDevice();
}
/**
 * @brief Construct a new Arduino:: Arduino object
 * 
 */
Arduino::Arduino(std::string portName, unsigned int buadRate, unsigned int timeout)
{
    this->portName = portName;
    this->buadRate = buadRate;
    this->timeout = timeout;
}

std::vector<std::string> Arduino::getFiles()
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
/**
 * @brief check the connection to the arduino and report.
 */
Arduino::ERROR_ARDUINO Arduino::checkConnection()
{
    if (!this->isOpen)
        return ERROR_ARDUINO::SUCCSESS;
    char helper = this->serial.openDevice(portName.c_str(), buadRate);
    if (helper != SUCCESS)
    {
        serial.closeDevice();
        return ERROR_ARDUINO::SUCCSESS;
    }
    serial.closeDevice();
    return ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief check the connection for debugging
 * 
 */
void Arduino::checkConnectionToConsole()
{
    if (checkConnection() != Arduino::ERROR_ARDUINO::SUCCSESS)
    {
        std::cout << "there was a problem with the connection" << std::endl;
    }
    std::cout << "the connection is successful!-" << std::endl;
}
/**
 * @brief add file to the files array
 */

void Arduino::addFile(std::string file)
{
    this->files.push_back(file);
}
/*
 * @brief will calculate the sum of a string
 * I forgot that I found this function on the web so I made 
 * a simple version of this using std::string
 * this one will probably be used in the arduino
 * @param message 
 * @return int 
 */
int Arduino::checkSum(char *message)
{
    int sum = 0;
    char *p = &message[0];
    while (*p != NULLCHAR)
    {
        sum += *message;
        ++p;
    }
    return sum;
}
/**
 * @brief open the serial class with the members of this class
 * @return will return the error if there is one. 
 */
Arduino::ERROR_ARDUINO Arduino::openSerial()
{
    int err = this->serial.openDevice(this->portName.c_str(), this->buadRate);
    if (err != SUCCESS)
        return ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return ERROR_ARDUINO::SUCCSESS;
}
/*
 * @brief send a serial commend to the arduino and confirm
 *  that the message has been received 
 * 
 * @param message  the commend to be sent, for now 3 is the prefix 
 * of the commends and the 2 digits behind it are the commend itself
 * 
 * @return the error code.
 */
Arduino::ERROR_ARDUINO Arduino::serialCommend(std::string message)
{
    if (!this->isOpen)
        return ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    char inputBuff[CHECKBUFFSIZE] = {0};
    this->serial.writeString(message.c_str());
    // check sum should be added.
    return ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief dumps data to file +time stamp
 * @param data the data from the arduino 
 * @param place from the file array
 */
Arduino::ERROR_ARDUINO Arduino::writeFromBufferToFile(std::string data, int place)
{
    std::ofstream dataLog; // this part could be one line but I want it that way.
    std::string fileName = "../dataFromArduino/sensor";
    std::string fileNameEnd = ".log";
    fileName.append(std::to_string(place));
    fileName.append(fileNameEnd);
    dataLog.open(fileName, std::ios_base::app);
    if (!dataLog)
    {
        return ERROR_ARDUINO::ERROR_WITH_FILES;
    }
    dataLog << std::time(0) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    return ERROR_ARDUINO::SUCCSESS;
}
Arduino::ERROR_ARDUINO Arduino::getDataWithWhileLoop()
{
    if (openSerial() == ERROR_WITH_SERIALPORT)
    {
        return ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    }
    while (true)
    {
        if (this->serial.available())
        {
            uint32_t num = 0;
            this->serial.readBytes(&num, sizeof(num));
            switch (num)
            {
            case 700:
                receiveDataFromSensor();
                continue;
            case 300:
                checkStatusFromArduino();
                continue;
            default:
                continue;
            }
            return ERROR_ARDUINO::SUCCSESS;
        }
    }
}
/**
 * @brief will receive a message and write it to the sensor log
 * 
 * @return the error message 
 */
Arduino::ERROR_ARDUINO Arduino::receiveDataFromSensor()
{
    uint32_t id = 0;
    this->serial.readBytes(&id, sizeof(id));
    float f = receiveFloat();
    this->writeFromBufferToFile(std::to_string(f), id);
    return ERROR_ARDUINO::SUCCSESS;
}
float Arduino::receiveFloat()
{
    float value = 0;
    this->serial.readBytes(&value, sizeof(value));
    return value;
}
/**
 * @brief for debugging, will add mock files for the logging
 * 
 * @return int 
 */
void Arduino::addLogFilesForSensors()
{
    this->addFile("..//dataFromArduino//sensor0.log");
    this->addFile("../dataFromArduino//sensor1.log");
    this->addFile("..//dataFromArduino//sensor2.log");
    this->addFile("..//dataFromArduino//sensor3.log");
    this->addFile("..//dataFromArduino//sensor4.log");
    this->addFile("..//dataFromArduino//sensor5.log");
}
/**
 * @brief the simplest way to receive a boolean from the arduino
 */
bool Arduino::receiveBoolean()
{
    bool value = 0;
    this->serial.readBytes(&value, sizeof(value));
    return value;
}
/**
 * @brief the function will send the arduino a message to 
 * start checking all of its modules. It will look for errors
 * and if there are it will reboot the arduino.
 * 
 * this is a wrapper.
 * 
 */
Arduino::ERROR_ARDUINO Arduino::checkStatusFromArduino()
{
    if (!receiveBoolean())
    {
        std::cout << "the com is broken!" << std::endl;
        return ERROR_ARDUINO::THE_ARDUINO_IS_NOT_RESPONDING;
    }
    else
    {
        std::cout << "the com is great!" << std::endl;
    }
    return ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief this is a wrapper for the message checking.
 *  the c++ language cant declare threaders from other scopes 
 * so I found a solution to this, just wrap the function inside the
 * scope of the arduino so it will be in this scope.
 *
 * I use here a lambda to make it make look prettier.
 * 
 * @return std::thread 
 */
std::thread Arduino::startTheArdCheking()
{
    return std::thread([=] { getDataWithWhileLoop(); });
}
std::string Arduino::exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}
