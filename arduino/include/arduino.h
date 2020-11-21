#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
class Arduino
{
public:
    Arduino(std::string fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout);
    Arduino();
    ~Arduino();
    int serialCommend(std::string messege);
    int writeFromBufferToFile(std::string messege, int place);
    int checkStatus();
    int checkConnection();
    void checkConnectionToConsule();
    int checkSum(char *messege); //TODO by mistake I made 2 instances of the same func
    int openSerial();
    int messegeCheckSum(std::string messege);
    int getSensorDataB(int Wsensor, int place); //big
    void addFile(std::string file);
private:
    serialib serial;
    std::ofstream fileToWrite;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    bool isOpen;
    std::vector< std::string > files;
};

// class ArduinoWithSensor : Arduino{
// public:
//     ArduinoWithSensor( std::ofstream &fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout,serialib serial,int sumTheLast24H);

//     int realTimeData();
// };