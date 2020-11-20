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
    int serialCom(std::string messege);

    int writeFromBufferToFile(std::string messege);

    int checkStatus();

    int checkConnection();
    
    void checkConnectionToConsule();
private:
    std::ofstream fileToWrite;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    serialib serial;

};

// class ArduinoWithSensor : Arduino{
// public:
//     ArduinoWithSensor( std::ofstream &fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout,serialib serial,int sumTheLast24H);
    
//     int realTimeData();
// };