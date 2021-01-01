#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
#include <chrono>
#include <mutex>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <memory>
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
        STATUS_IS_BAD,
    };

    Arduino(const std::string &portName, unsigned int buadRate, unsigned int timeout);
    ~Arduino();

    void setTimeout(int timeout);
    int getTimout();
    void setFiles();
    const std::vector<std::string> &getFiles();
    void checkConnectionToConsole();
    int checkSum(const char *message); //TODO by mistake I made 2 instances of the same func
    Arduino::ERROR_ARDUINO openSerial();
    void addFile(const std::string &file);
    bool receiveBoolean();
    Arduino::ERROR_ARDUINO getDataWithWhileLoop();
    void addLogFilesForSensors();
    std::thread startTheArduinoCheking();

private:
    Arduino::ERROR_ARDUINO checkStatus(int delay);
    Arduino::ERROR_ARDUINO checkConnection();
    Arduino::ERROR_ARDUINO serialCommend(const std::string &message);
Arduino::ERROR_ARDUINO writeFromBufferToFile(const std::string& data, int place);
    Arduino::ERROR_ARDUINO checkStatusFromArduino();
    Arduino::ERROR_ARDUINO startCheckingForMessege();
    int getCheckSumFromMessage(std::string message);
    Arduino::ERROR_ARDUINO receiveDataFromSensor();
    float receiveFloat();

    std::ofstream fileToWrite;
    serialib serial;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    bool isOpen;
    std::vector<std::string> files;
};
