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
#include <ctime>
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
Arduino::ERRORARDUINO Arduino::openSerial()
{
    int err = this->serial.openDevice(this->portName.c_str(), this->buadRate);
    if (err != SUCCESS)
        return Arduino::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return Arduino::SUCCSESS;
}
/**
 * @brief check the sum of the string
 * 
 * @param messege 
 * @return int sum of the string
 */
int Arduino::messageCheckSum(std::string messege)
{
    int sum = 0;
    for (int i = 0; i < messege.length(); i++)
    {
        if (messege[i] == '!')
            break;
        sum += messege[i];
    }
    return sum;
}
/**
 * @brief send a serial commend to the arduino and confirm
 *  that the messege has been recived 
 * 
 * @param messege  the commend to be sent, for now 3 is the prifx 
 * of the commends and the 2 digits behind it are the commend itself
 * 
 * @return int the Confirmation of the messege
 *  1=SUCCESS
 * -2=the messege wasnt received propraly
 * -1=the serial object wasnt opend
 * //TODO add a pretty table
 */
Arduino::ERRORARDUINO Arduino::serialCommend(std::string messege)
{
    if (!this->isOpen)
        return Arduino::ERRORARDUINO::ERROR_WITH_SERIALPORT;
    char inputBuff[CHECKBUFFSIZE] = {0};
    //messege.append(std::to_string(messageCheckSum(messege)));
    //messege.append(" ");
    this->serial.writeString(messege.c_str());
    //sleep(5000)
    this->serial.readString(inputBuff, '\n', READLENGTH, TIMEOUT);
    std::string approver = inputBuff;
    approver = approver.substr(0, 3); //TODO should i make this a constant
    if (inputBuff != messege)
        return Arduino::ERRORARDUINO::THE_CHECKSUM_DIDNT_WORK; // TODO check if the arduino will return the exact string that i send
    //+ if it will send the \n
    return Arduino::ERRORARDUINO::SUCCSESS;
}
/**
 * @brief will get a data dump of the arduino to a file 
 * the func will send a commend to the arduino to start sending the data
 * and then will recive READLENGTH of bytes. the func will write them to the file
 * that is privided.
 * the problem right now is that it has only 1000 bytes of data.
 * @param Wsensor which sensor
 * @param place the place in the array of the files
 * @return int the status of result
 */
Arduino::ERRORARDUINO Arduino::getSensorDataB(int Wsensor, int place)
// TODO maby the sensor param is stupid.
{
    if (!this->isOpen)
        return Arduino::ERRORARDUINO::ERROR_WITH_SERIALPORT; // TODO it will check twice once inside this func and once inside the serialCommend.
    std::string commendToArduino = std::to_string(Arduino::HEADERS::ASK_FOR_STATUS_PER_SENSOR);
    commendToArduino.append(std::to_string(Wsensor)); // the protocol works like http (more on that in protocol doc)
    if (serialCommend(commendToArduino) != Arduino::ERRORARDUINO::ERROR_WITH_SERIALPORT)
        ;
    {
        return Arduino::ERRORARDUINO::ERROR_WITH_SERIALPORT;
    }
    char data[DATALEN] = {0};                                 // TODO   check if 1000 is enough.
    this->serial.readString(data, '\n', READLENGTH, TIMEOUT); // TODO the timout can be for the entire send,
    //that can cause the readString func to return without all the data
    writeFromBufferToFile(data, place);
    return Arduino::ERRORARDUINO::SUCCSESS;
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
int Arduino::getCheckSumFromMessage(std::string massage)
{
    std::size_t foundExamation = massage.find('!');
    std::size_t foundn = massage.find('\n');
    if (foundn == std::string::npos)
    {
        // the end line wasnt recicved well
    }
    if (foundExamation == std::string::npos)
    {
        // the ! was not found
    }
    std::string checkString = massage.substr(foundExamation + 1, foundn - (foundExamation + 1));
    // TOD-1 meby put all of this to a function
    //~foundN - foundExamation~ the distance between the satrt and the end.
    // this way it will give a positive result
    std::string::size_type sz; // alias of size_t
    int checkSumDecimal = std::stoi(checkString, &sz, 10);
    return checkSumDecimal;
}
int Arduino::receiveData(std::string message)
{
    bool correct = true;
    bool exit = false;
    serialCommend("600");
    while (!exit)
    {
        std::string temp;              // TODO maby just put the decleration outside the while scope
        char buffer[READLENGTH] = {0}; //100
        //TODO the checksum is non inplemented as intended
        this->serial.readString(buffer, '\n', READLENGTH, 0);
        temp = buffer;
        if (temp.find("end send") != std::string::npos)
        {
            //i dont need to check the integrity of the messege if its just the end
            break;
        }
        if (getCheckSumFromMessage(temp) != messageCheckSum(temp))
        {
            correct = false;
            //TODO implement a system to resend the last line
            serialCommend("602"); // send the last one another time
        }
        else
        {
            correct = true;
            message.push_back(*buffer);
            serialCommend("601"); // send the next one
        }
    }
}
//  char * p = buffer;
//   for (int i = 0; i < ciwccccc; i++)
//   {
//       /* code */
//   }
int Arduino::receiveMessage()
{
    // char arr[100];
    // this->serial.readBytes(&arr, 100);
    uint32_t num = 0;
    this->serial.readBytes(&num, sizeof(num));
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

int Arduino::addLogFilesForSensors()
{
    this->addFile("..//dataFromArduino//sensor0.log");
    this->addFile("../dataFromArduino//sensor1.log");
    this->addFile("..//dataFromArduino//sensor2.log");
    this->addFile("..//dataFromArduino//sensor3.log");
    this->addFile("..//dataFromArduino//sensor4.log");
    this->addFile("..//dataFromArduino//sensor5.log");
}