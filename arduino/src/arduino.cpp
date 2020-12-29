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
    @brief construct the arduino class for the testing
*/
Arduino::Arduino()
{
    this->portName = "/dev/ttyUSB0";
    this->buadRate = RATE;
    this->timeout = TIMEOUT;
}
/*!
 * @brief Destroy the Arduino:: Arduino object 
 * probably meaningless 

 */
Arduino::~Arduino()
{
    this->serial.closeDevice();
    this->isOpen = false;
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
        return Arduino::ERROR_ARDUINO::SUCCSESS;
    char helper = this->serial.openDevice(portName.c_str(), buadRate);
    if (helper != SUCCESS)
    {
        serial.closeDevice();
        return Arduino::ERROR_ARDUINO::SUCCSESS;
    }
    serial.closeDevice();
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief check the connection for debugging
 * 
 */
void Arduino::checkConnectionToConsole()
{
    if (checkConnection() != Arduino::ERROR_ARDUINO::SUCCSESS)
    {
        std::cout << "there was a problem with the connection"<< std::endl;
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
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return Arduino::ERROR_ARDUINO::SUCCSESS;
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
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    char inputBuff[CHECKBUFFSIZE] = {0};
    this->serial.writeString(message.c_str());
    this->serial.readString(inputBuff, '\n', READLENGTH, TIMEOUT);
    std::string approver = inputBuff;
    approver = approver.substr(0, 3);
    // check sum should be added.
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief dumps data to file +time stamp
 * @param data the data from the arduino 
 * @param place from the file array
 * @return int 
 */
int Arduino::writeFromBufferToFile(std::string data, int place)
{
    std::ofstream dataLog;
    //TODO   I just couldn't open it with the relative path
    dataLog.open("..//dataFromArduino//sensor4.log");
    if (!dataLog)
    {
        std::cerr << "Could not open the file!" << std::endl;
        std::cout << system("ls") << std::endl;
    }
    dataLog << std::time(0) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    return SUCCESS;
}
Arduino::ERROR_ARDUINO  Arduino::getDataWithWhileLoop()
{
    if (openSerial() == Arduino::ERROR_WITH_SERIALPORT)
    {
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    }
    int counter = 0;
    std::cout << "entering the while loop" << std::endl;
    while (true)
    {
        if (this->serial.available())
        {
            counter++;
            std::cout << counter << std::endl;
            uint32_t num = 0;
            this->serial.readBytes(&num, sizeof(num));
            switch (num)
            {
            case 700:
                receiveMessage();
                continue;
            case 300:
                checkStatusFromArduino();
                continue;
            default:
                continue;
            }
            return Arduino::ERROR_ARDUINO::SUCCSESS;
        }
    }
}
/**
 * @brief will receive a message and write it to the sensor log
 * 
 * @return the error message 
 */
Arduino::ERROR_ARDUINO Arduino::receiveMessage()
{
    uint32_t id = 0;
    this->serial.readBytes(&id, sizeof(id));

    if (id == Arduino::SENSOR4)
    {
        float f = receiveFloat();
        this->writeFromBufferToFile(std::to_string(f), id);
    }
    return ERROR_ARDUINO::SUCCSESS 
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
        return Arduino::ERROR_ARDUINO::THE_ARDUINO_IS_NOT_RESPONDING;
    }
    else
    {
        std::cout << "the com is great!" << std::endl;
        Arduino::ERROR_ARDUINO::SUCCSESS;
    }
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
