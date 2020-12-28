#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
#include <chrono>
#include <mutex>
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
    enum ERROR_ARDUINO
    {
        SUCCSESS = 0,
        ERROR_WITH_SERIALPORT,
        ERROR_WITH_FILES,
        THE_CHECKSUM_DIDNT_WORK,
        THE_ARDUINO_IS_NOT_RESPONDING,
    };
    Arduino(std::string fileToWrite, std::string portName, unsigned int buadRate, unsigned int timeout);
    Arduino();
    ~Arduino();
    void setTimeout(int timeout);
    int getTimout();
    void setFiles();
    std::vector<std::string> getFiles();
    Arduino::ERROR_ARDUINO serialCommend(std::string messege);
    int writeFromBufferToFile(std::string messege, int place);
    Arduino::ERROR_ARDUINO checkStatus(int delay);
    int checkConnection();
    Arduino::ERROR_ARDUINO checkStatusFromArduino();
    void checkConnectionToConsule();
    int checkSum(char *messege); //TODO by mistake I made 2 instances of the same func
    Arduino::ERROR_ARDUINO openSerial();
    int messageCheckSum(std::string messege);
    Arduino::ERROR_ARDUINO getSensorDataB(int Wsensor, int place); //big dump.
    void addFile(std::string file);
    bool receiveBoolean();
    int getDataWithWhileLoop();
    // Arduino::ERRORARDUINO getDataWithWhileLoop();
    int getCheckSumFromMessage(std::string message);
    Arduino::ERROR_ARDUINO receiveMessage();
    float receiveFloat();
    int addLogFilesForSensors();
    Arduino::ERROR_ARDUINO startCheckingForMessege();
    std::thread startTheArdCheking();
    //for the testing↓
    serialib serial;

private:
    std::ofstream fileToWrite;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    bool isOpen;
    std::vector<std::string> files;
    // std::vector<std::counting_semaphore<1>()> fileSaver;
};

