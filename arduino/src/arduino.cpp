#include "arduino.h"
#include <ctime>
#define RATE 9600
#define TIMEOUT 100000
#define READLENGTH 100

Arduino::Arduino()
{
    this->portName = "/dev/ttyUSB0";
    this->buadRate = 9600;
    this->timeout = 10000;
}
Arduino::~Arduino()
{
    this->serial.closeDevice();
    this->isOpen = false;
}

Arduino::Arduino(std::string fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout)
{
    Arduino::fileToWrite.open(fileToWrite.c_str());
}
int Arduino::checkConnection()
{
    char helper = this->serial.openDevice(portName.c_str(), buadRate);
    if (helper != 1)
    {
        serial.closeDevice();
        return helper;
    }
    serial.closeDevice();
    return helper;
}
void Arduino::checkConnectionToConsule()
{
    int status = checkConnection();
    if (status != 1)
    {
        std::cout << "there was a problem with the connection, the problen code is- " << status << std::endl;
    }
    std::cout << "the connection is successful!-" << status << "-" << std::endl;
}
void Arduino::addFile(std::string file)
{
    this->files.push_back(file);
}
int Arduino::checkSum(char *messege)
{
    int sum = 0;
    char *p = &messege[0];
    while (*p != '\0')
    {
        sum += *messege;
        ++p;
    }
    return sum;
}

int Arduino::openSerial()
{
    int err = this->serial.openDevice(portName.c_str(), buadRate);
    if (err != 1)
        return -1;
    this->isOpen = true;
    return 1;
}
int Arduino::messegeCheckSum(std::string messege)
{
    int sum = 0;
    for (int i = 0; i < messege.length(); i++)
        sum += messege[i];
    return sum;
}
int Arduino::serialCommend(std::string messege)
{
    if (!this->isOpen)
        return -1;
    char inputBuff[64] = {0};
    messege.append(std::to_string(messegeCheckSum(messege)));
    messege.append(" ");
    this->serial.writeString(messege.c_str());
    //sleep(5000)
    this->serial.readString(inputBuff, '\n', 100, 10000);
    if (inputBuff != messege)
        return -2; // TODO check if the arduino will return the exact string that i send + if it will send the \n
    return 1;
}
int Arduino::getSensorDataB(int Wsensor, int place)
{
    if (!this->isOpen)
        return -1; // it will check twice once inside this func and once inside the serialCommend.
    std::string commendToArduino = "30";
    commendToArduino.append(std::to_string(Wsensor)); // the protocol works like http (more on that in protocol doc)
    int err = serialCommend(commendToArduino);
    if (err != 1)
    { // the func didnt seecseed
        return -1;
    }
    char data[1000] = {0};
    this->serial.readString(data, '\n', 100, 10000);
    writeFromBufferToFile(data, place);
    return 1;
}
int Arduino::writeFromBufferToFile(std::string data, int place)
{
    std::ofstream dataLog;
    // const char* name = this->files[place].c_str();
    dataLog.open(this->files[place], std::ios::app);
    dataLog << std::time(0) << std::endl;
    dataLog << data << std::endl;
    dataLog.close();
}