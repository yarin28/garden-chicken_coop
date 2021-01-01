#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
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

    enum ERROR_ARDUINO
    {
        SUCCSESS = 0,
        ERROR_WITH_SERIALPORT,
        ERROR_WITH_FILES,
        THE_CHECKSUM_DIDNT_WORK,
        THE_ARDUINO_IS_NOT_RESPONDING,
        STATUS_IS_BAD,
    };

    /**
 * @brief Construct a new Arduino::Arduino object
 * 
 */
    Arduino(const std::string &portName, unsigned int buadRate, unsigned int timeout);

    /*!
 * @brief Destroy the Arduino:: Arduino object 
 * probably meaningless 
 */
    ~Arduino();

    /**
 * @brief setter of the timeout
 * 
 * @param timeout int the time to abandon the serial communication
 */
    void setTimeout(int timeout);

    /**
 * @brief getter for the timeout of the serial port
 * 
 * @return int the time to abandon the serial communication
 */
    int getTimout();

    /**
 * @brief getter, will return an array of files that the class uses
 * to output of the sensors
 * 
 * @return std::vector<std::string> an array of string 
 */
    void setFiles();
    const std::vector<std::string> &getFiles();
    /**
 * @brief check the connection 
 */
    void checkConnectionToConsole();
    /*
 * @brief calculate the sum of a string
 * @param message a string that the checksum will be calcualted from
 * @return int the checksum that was calculated from the message
 */
    int checkSum(const char *message); //TODO by mistake I made 2 instances of the same func
                                       /**
 * @brief open the serial class with the members of this class
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. or succsess 
 */
    Arduino::ERROR_ARDUINO openSerial();
    /**
 * @brief adds file to the files array
 * @param file the file to add to the array of files
 */
    void addFile(const std::string &file);
    /**
 * @brief the simplest way to receive a boolean from the arduino
 * @return bool the value that came from the serial port.
 */
    bool receiveBoolean();
    /**
 * @brief this is the main function,
 * will listen to the serial port and interact with it.
 * 
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
    Arduino::ERROR_ARDUINO getDataWithWhileLoop();
    /**
 * @brief will add mock files for the logging
 */
    void addLogFilesForSensors();
    /**
 * @brief this is a wrapper for the message checking.
 * @return std::thread the thread of the fanction
 */
    std::thread *startTheArduinoCheking();

private:
    /**
 * @brief the function will send the arduino a message to 
 * start checking all of its modules. 
 */
    Arduino::ERROR_ARDUINO checkStatus(int delay);

    int receiveInt();

    /**
 * @brief check the connection to the arduino and report.
 * @return the error code 
 */
    Arduino::ERROR_ARDUINO checkConnection();

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
    Arduino::ERROR_ARDUINO serialCommend(const std::string &message);

    /**
 * @brief dumps data and timestamp to file
 * @param data to write to the file
 * @param place the index of the file from array
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
    Arduino::ERROR_ARDUINO writeFromBufferToFile(const std::string &data, int place);

    Arduino::ERROR_ARDUINO checkStatusFromArduino();

    int getCheckSumFromMessage(std::string message);
    /**
 * @brief will receive a message and write it to the sensor log
 * @return Arduino::ERROR_ARDUINO will return the error code if there is one. 
 */
    Arduino::ERROR_ARDUINO receiveFloatFromSensor();
    /**
 * @brief will receive float from serial port
 * 
 * @return float the value that came out.
 */
    float receiveFloat();

    std::ofstream fileToWrite;
    serialib serial;
    std::string portName;
    unsigned int buadRate;
    unsigned int timeout;
    bool isOpen;
    std::vector<std::string> files;
};
