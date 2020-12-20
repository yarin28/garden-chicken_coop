#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
class Arduino
{
public:
    enum HEADERS
    {
        GET_FLOAT = 700,
        GET_BOOLEAN = 701,
        ASK_FOR_STATUS_PER_SENSOR = 30 /* the sensor*/,
        ASK_FOR_OVERALL_STATUS = 300,

    };
    enum ID : unsigned char // to make the fileWriting simpler
    {
        SENSOR0,
        SENSOR1,
        SENSOR2,
        SENSOR3,
        SENSOR4,
        SENSOR5,
    };
    Arduino(std::string fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout);
    Arduino();
    ~Arduino();
    void setTimeout(int timeout);
    int getTimout();
    void setFiles();
    std::vector<std::string> getFiles();
    int serialCommend(std::string messege);
    int writeFromBufferToFile(std::string messege, int place);
    int checkStatus();
    int checkConnection();
    void checkConnectionToConsule();
    int checkSum(char *messege); //TODO by mistake I made 2 instances of the same func
    int openSerial();
    int messageCheckSum(std::string messege);
    int getSensorDataB(int Wsensor, int place); //big dump.
    void addFile(std::string file);
    int receiveData(std::string message);
    int getCheckSumFromMessage(std::string message);
    int receiveMessage();
    float receiveFloat();
    int addLogFilesForSensors();
    //for the testing↓
    serialib serial;

private:
    std::ofstream fileToWrite;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    bool isOpen;
    std::vector<std::string> files;
};

// class ArduinoWithSensor : Arduino{
// public:
//     ArduinoWithSensor( std::ofstream &fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout,serialib serial,int sumTheLast24H);

//     int realTimeData();
// };