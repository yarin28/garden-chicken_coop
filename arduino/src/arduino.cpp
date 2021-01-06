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

ArduinoClient::ArduinoClient(const std::string &portName, unsigned int baudRate, unsigned int timeout) : portName(portName), baudRate(baudRate), timeout(timeout)
{
}

ArduinoClient::~ArduinoClient()
{
    this->serial.closeDevice();
}

int ArduinoClient::getTimout()
{
    return this->timeout;
}

void ArduinoClient::setTimeout(int timeout)
{
    this->timeout = timeout;
}

ArduinoClient::ERROR ArduinoClient::checkConnection()
{
    if (this->isOpen)
        return ArduinoClient::ERROR::SUCCSESS;
    char errorCode = serial.openDevice(portName.c_str(), baudRate);
    if (errorCode != 1)
    {
        return ArduinoClient::ERROR::ERROR_WITH_SERIALPORT;
    }
    return ArduinoClient::ERROR::SUCCSESS;
}

int ArduinoClient::checkSum(const char *message) const
{
    int sum = 0;
    const char *p = &message[0];
    while (*p != 0)
    {
        sum += *message;
        ++p;
    }
    return sum;
}

ArduinoClient::ERROR ArduinoClient::openSerial()
{
    int err = serial.openDevice(this->portName.c_str(), this->baudRate);
    if (err != 1)
        return ArduinoClient::ERROR::ERROR_WITH_SERIALPORT;
    this->isOpen = true;
    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::serialCommend(const std::string &message)
{
    if (!this->isOpen)
        return ArduinoClient::ERROR::CLOSED_CONNECTION;
    serial.writeString(message.c_str());
    // check sum should be added.
    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::getDataWithWhileLoop()
{
    ArduinoClient::ERROR return_code = ArduinoClient::ERROR::SUCCSESS;
    return_code = openSerial();
    if (return_code != ArduinoClient::ERROR::SUCCSESS)
    {
        goto l_cleanup;
    }
    while (true)
    {
        if (serial.available())
        {
            uint32_t command = 0;
            receiveInt(command);
            switch (command)
            {
            case ArduinoClient::MESSAGES::FLOAT_DATA:
                return_code = logFloatFromSensor();
                if (return_code != ArduinoClient::ERROR::SUCCSESS)
                    goto l_cleanup;
                continue;
            case ArduinoClient::COMMANDS::ASK_FOR_OVERALL_STATUS:
                return_code = checkStatusFromArduino();
                if (return_code != ArduinoClient::ERROR::SUCCSESS)
                    goto l_cleanup;
                continue;
            default:
                continue;
            }
        }
    }

l_cleanup:
    return return_code;
}

ArduinoClient::ERROR ArduinoClient::logFloatFromSensor()
{
    ArduinoClient::ERROR errorCode = ArduinoClient::ERROR::SUCCSESS;
    uint32_t sensorId = 0;
    receiveInt(sensorId);
    float value = 0;
    receiveFloat(value);

    FileHandler::ERROR errorCodeFile = FileHandler::ERROR::SUCCSESS;
    FileHandler file_array_temp;
    errorCodeFile = file_array_temp.writeFromBufferToFile(std::to_string(value), sensorId);
    return errorCode;
}

ArduinoClient::ERROR ArduinoClient::receiveFloat(float &value)
{
    int error_code = serial.readBytes(&value, sizeof(value));
    if (error_code == 1)
        return ArduinoClient::ERROR::ERROR_WITH_SERIALPORT;
    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::receiveBoolean(uint8_t &value)
{
    if (serial.readBytes(&value, sizeof(value)) <= 0)
    {
        return ArduinoClient::ERROR::ERROR_WITH_SERIALPORT;
    }

    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::checkStatusFromArduino()
{
    uint8_t isGood = 0;
    receiveBoolean(isGood);
    if (isGood == 1)
    {
        std::cout << "the Status is great!" << std::endl;
    }
    else
    {
        std::cout << "the Status is bad!" << std::endl;
        return ArduinoClient::ERROR::BAD_STATUS;
    }

    return ArduinoClient::ERROR::SUCCSESS;
}

std::thread *ArduinoClient::startTheArduinoCheking()
{
    return new std::thread([=] { getDataWithWhileLoop(); });
}

ArduinoClient::ERROR ArduinoClient::receiveInt(uint32_t &value)
{
    if (serial.readBytes(&value, sizeof(value)) <= 0)
    {
        return ArduinoClient::ERROR::ERROR_WITH_SERIALPORT;
    }
    return ArduinoClient::ERROR::SUCCSESS;
}
