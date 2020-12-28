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
    @brief constract the arduino class for the testing
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
 * //TODO the meaning of this is to be determined

 */
Arduino::~Arduino()
{
    this->serial.closeDevice();
    this->isOpen = false;
}
/**
 * @brief Construct a new Arduino:: Arduino object
 * 
 * @param fileToWrite 
 * @param portName 
 * @param buadRate 
 * @param timeout 
 */
Arduino::Arduino(std::string fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout)
{
    this->fileToWrite.open(fileToWrite.c_str()); //TODO this parameter is usless
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
 * 
 * @return  1 -> success
 *          -1-> faild 
 */
int Arduino::checkConnection()
{
    if (!this->isOpen)
        return SUCCESS;
    char helper = this->serial.openDevice(portName.c_str(), buadRate);
    if (helper != SUCCESS)
    {
        serial.closeDevice();
        return helper;
    }
    serial.closeDevice();
    return helper;
}
/**
 * @brief check the connection for debugging
 * 
 */
void Arduino::checkConnectionToConsule()
{
    int status = checkConnection();
    if (status != SUCCESS)
    {
        std::cout << "there was a problem with the connection, the problen code is- " << status << std::endl;
    }
    std::cout << "the connection is successful!-" << status << "-" << std::endl;
}
/**
 * @brief add file to the files array
 * 
 * @param file 
 */

void Arduino::addFile(std::string file)
{
    this->files.push_back(file);
}
/**
 * @brief will caculate the sum of a string
 * i forgot that i found this func on the web so i made 
 * a simple version of this useing std::string
 * this one will probaby be used in the arduino
 * @param messege 
 * @return int 
 */
int Arduino::checkSum(char *messege)
{
    int sum = 0;
    char *p = &messege[0];
    while (*p != NULLCHAR)
    {
        sum += *messege;
        ++p;
    }
    return sum;
}
/**
 * @brief open the serial class with the members of this class
 * 
 * @return will return the error if itere is one. 
 */
Arduino::ERROR_ARDUINO Arduino::openSerial()
{
    int err = this->serial.openDevice(this->portName.c_str(), this->buadRate);
    if (err != SUCCESS)
        return Arduino::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return Arduino::SUCCSESS;
}
/**
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
 * @brief dumps data to file +timestamp
 * @param data 
 * @param place from the file array
 * @return int 
 */
int Arduino::writeFromBufferToFile(std::string data, int place)
{
    std::ofstream dataLog;
    // const char* name = this->files[place].c_str();
    // dataLog.open(this->files[place].c_str(), std::ios::app);
    //TODO   i just couldnt open it with the relative path
    dataLog.open("/home/pi/garden-chicken_coop/arduino/dataFromArduino/sensor4.log", std::ios::app);
    if (!dataLog)
    {
        std::cerr << "Could not open the file!" << std::endl;
        std::cout << system("ls") << std::endl;
    }
    // TODO the time ↓ cannot be trusted
    dataLog << std::time(0) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
    return SUCCESS;
}
int Arduino::getDataWithWhileLoop()
{
    if (openSerial() == Arduino::ERROR_WITH_SERIALPORT)
    {
        // return Arduino::ERROR_ARDUINO::ERROR_WITH_SERIALPORT;
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
 * @brief will recive a message and write it to the sensor log
 * 
 * @return the error message 
 */
Arduino::ERROR_ARDUINO Arduino::receiveMessage()
{
    // char arr[100];
    // this->serial.readBytes(&arr, 100);
    /* implemented this in the reciveData func
    // uint32_t num = 0; 
     this->serial.readBytes(&num, sizeof(num));
     */
    // the id is for which log file to put the log in
    uint32_t id = 0;
    this->serial.readBytes(&id, sizeof(id));

    if (id == Arduino::SENSOR4) //TODO maby change the enum
    // this is to check if the writing is relable.
    {
        float f = receiveFloat();
        this->writeFromBufferToFile(std::to_string(f), id);
    }
}
float Arduino::receiveFloat()
{
    float value = 0;
    this->serial.readBytes(&value, sizeof(value));
    return value;
}
/**
 * @brief for debuging, will add mockfiles for the logging
 * 
 * @return int 
 */
int Arduino::addLogFilesForSensors()
{
    this->addFile("..//dataFromArduino//sensor0.log");
    this->addFile("../dataFromArduino//sensor1.log");
    this->addFile("..//dataFromArduino//sensor2.log");
    this->addFile("..//dataFromArduino//sensor3.log");
    this->addFile("..//dataFromArduino//sensor4.log");
    this->addFile("..//dataFromArduino//sensor5.log");
}
/**
 * @brief the sipmlest way to recive a boolean from the arduino
 * 
 * //TODO check the func!!!
 * 
 * @return true 
 * @return false 
 */
bool Arduino::receiveBoolean()
{
    bool value = 0;
    this->serial.readBytes(&value, sizeof(value));
    return value;
}
/**
 * @brief the function will send the arduino a message to 
 * start checking all of its modules. it will look for errors
 * and if there are it will reboot the arduino.
 * 
 * this is a wraper.
 * 
 * @return Arduino::ERROR_ARDUINO 
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
 * @brief this is a wrapper for the messege checking.
 *  the c++ languich cant declare threades from other scopes 
 * so i found a sulution to this, just wrap the fanction inside the
 * scope of the arduino so it will be in this scope.
 *
 * i use here a lambda to make it make look prettier.
 * 
 * @return std::thread 
 */
std::thread Arduino::startTheArdCheking()
{
    // std::cout<<"in the arduino cheking"<<std::endl;
    return std::thread([=] { getDataWithWhileLoop(); });
}
