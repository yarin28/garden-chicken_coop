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
#define CHECK_BUFFER_SIZE 64
#define FILE_NAME "../dataFromArduino/sensor"
#define FILE_NAME_END ".log"
/*!
 * @brief Destroy the Arduino:: Arduino object 
 * probably meaningless 
 */
Arduino::~Arduino()
{
    this->serial.closeDevice();
}
/**
 * @brief Construct a new Arduino::Arduino object
 * 
 */
Arduino::Arduino(const std::string &portName, unsigned int buadRate, unsigned int timeout) : portName(portName), buadRate(buadRate), timeout(timeout)
{
}
/**
 * @brief getter, will return an array of files that the class uses
 * to output of the sensors
 * 
 * @return std::vector<std::string> an array of string 
 */
const std::vector<std::string> &Arduino::getFiles()
{
    return this->files;
}
/**
 * @brief getter for the timeout of the serial port
 * 
 * @return int the time to abandon the serial communication
 */
int Arduino::getTimout()
{
    return this->timeout;
}
/**
 * @brief setter of the timeout
 * 
 * @param timeout int the time to abandon the serial communication
 */
void Arduino::setTimeout(int timeout)
{
    this->timeout = timeout;
}
/**
 * @brief check the connection to the arduino and report.
 * @return the error code 
 */
Arduino::ERROR_ARDUINO Arduino::checkConnection()
{
    if (!this->isOpen)
        return Arduino::ERROR_ARDUINO::SUCCSESS;
    char helper = this->serial.openDevice(portName.c_str(), buadRate);
    if (helper != 1)
    {
        serial.closeDevice();
    }
    serial.closeDevice();
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief check the connection 
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
 * @brief adds file to the files array
 * @param file the file to add to the array of files
 */

void Arduino::addFile(const std::string &file)
{
    this->files.push_back(file);
}
/*
 * @brief calculate the sum of a string
 * @param message a string that the checksum will be calcualted from
 * @return int the checksum that was calculated from the message
 */
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
/**
 * @brief open the serial class with the members of this class
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. or succsess 
 */
Arduino::ERROR_ARDUINO Arduino::openSerial()
{
    int err = this->serial.openDevice(this->portName.c_str(), this->buadRate);
    if (err != Arduino::ERROR_ARDUINO::SUCCSESS)
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/*
 * @brief send a serial commend to the arduino and confirm
 *  that the message has been received 
 * 
 * @param message  the commend to be sent, now 3 is the prefix 
 * of the commends and the 2 digits behind it are the commend itself
 * for example 301
 * 3->the prifix (this message will be a command)
 * 01-> the "id" (this one will request status from sensor 1)
 * 
 * @return Arduino::ERROR_ARDUINO the error code.
 */
Arduino::ERROR_ARDUINO Arduino::serialCommend(const std::string &message)
{
    if (!this->isOpen)
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    this->serial.writeString(message.c_str());
    // check sum should be added.
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief dumps data and timestamp to file
 * @param data to write to the file
 * @param place the index of the file from array
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
Arduino::ERROR_ARDUINO Arduino::writeFromBufferToFile(const std::string &data, int place)
{
    std::ofstream dataLog; // this part could be one line but I want it that way.
    std::string fileName = FILE_NAME;
    fileName.append(std::to_string(place));
    fileName.append(FILE_NAME_END);
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
/**
 * @brief this is the main function,
 * will listen to the serial port and interact with it.
 * 
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
Arduino::ERROR_ARDUINO Arduino::getDataWithWhileLoop()
{
    if (this->openSerial() == Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT)
    {
        return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
    }
    while (true)
    {
        if (this->serial.available())
        {
            uint32_t num = 0;
            this->serial.readBytes(&num, sizeof(num));
            switch (num)
            {
            case Arduino::HEADERS::GET_FLOAT:
                receiveDataFromSensor();
                continue;
            case Arduino::HEADERS::ASK_FOR_OVERALL_STATUS:
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
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
Arduino::ERROR_ARDUINO Arduino::receiveDataFromSensor()
{
    uint32_t id = 0;
    this->serial.readBytes(&id, sizeof(id));
    float f = receiveFloat();
    this->writeFromBufferToFile(std::to_string(f), id);
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief will receive float from serial port
 * 
 * @return float the value that came out.
 */
float Arduino::receiveFloat()
{
    float value = 0;
    this->serial.readBytes(&value, sizeof(value));
    return value;
}
/**
 * @brief will add mock files for the logging
 */
void Arduino::addLogFilesForSensors()
{
    this->addFile("..//dataFromArduino//sensor0.log");
    this->addFile("..//dataFromArduino//sensor1.log");
    this->addFile("..//dataFromArduino//sensor2.log");
    this->addFile("..//dataFromArduino//sensor3.log");
    this->addFile("..//dataFromArduino//sensor4.log");
    this->addFile("..//dataFromArduino//sensor5.log");
}
/**
 * @brief the simplest way to receive a boolean from the arduino
 * @return bool the value that came from the serial port.
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
    if (receiveBoolean())
    {
        std::cout << "the Status is great!" << std::endl;
    }
    else
    {
        std::cout << "the Status is bad!" << std::endl;
        return Arduino::ERROR_ARDUINO::STATUS_IS_BAD;
    }
    return Arduino::ERROR_ARDUINO::SUCCSESS;
}
/**
 * @brief this is a wrapper for the message checking.
 * @return std::thread the thread of the fanction
 */
std::thread Arduino::startTheArduinoCheking()
{
    return std::thread([=] { getDataWithWhileLoop(); });
}
