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
#define SERIALLIB_SUCCESS 1

ArduinoClient::ArduinoClient(const std::string &portName, unsigned int baudRate, unsigned int timeout, DataRciveCallback dataRciveCallback) : portName(portName), baudRate(baudRate), timeout(timeout), dataRciveCallback(dataRciveCallback)
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
        return ArduinoClient::ERROR::SERIAL_PORT_ERROR;
    this->isOpen = true;
    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::sendSerialCommend(const std::string &message)
{
    if (!this->isOpen)
        return ArduinoClient::ERROR::CLOSED_CONNECTION;
    serial.writeString(message.c_str());
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
            uint32_t message = 0;
            return_code = receiveInt(message);
            if (return_code != ArduinoClient::ERROR::SUCCSESS)
                goto l_cleanup;
            switch (message)
            {
            case ArduinoClient::MESSAGES::FLOAT_DATA:
                if (return_code != ArduinoClient::ERROR::SUCCSESS)
                    goto l_cleanup;
                continue;
            case ArduinoClient::COMMANDS::ASK_FOR_OVERALL_STATUS:
                return_code = printStatusFromArduino();
                if (return_code != ArduinoClient::ERROR::SUCCSESS)
                    goto l_cleanup;
                continue;
            default:
                continue;
            }
        }
    }

l_cleanup:
    serial.closeDevice();
    return return_code;
}

ArduinoClient::ERROR ArduinoClient::logFloatFromSensor()
{
    ArduinoClient::ERROR errorCode = ArduinoClient::ERROR::SUCCSESS;
    uint32_t sensorId = 0;
    errorCode = receiveInt(sensorId);
    if (errorCode != ArduinoClient::ERROR::SUCCSESS)
        return errorCode;
    float value = 0;
    errorCode = receiveFloat(value);
    if (errorCode != ArduinoClient::ERROR::SUCCSESS)
        return errorCode;
    dataRciveCallback(sensorId, value);
    return errorCode;
}
ArduinoClient::ERROR ArduinoClient::receiveFloat(float &value)
{
    int error_code = serial.readBytes(&value, sizeof(value));
    if (error_code == 1)
        return ArduinoClient::ERROR::SERIAL_PORT_ERROR;
    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::receiveBoolean(uint8_t &value)
{
    if (serial.readBytes(&value, sizeof(value)) <= 0)
    {
        return ArduinoClient::ERROR::SERIAL_PORT_ERROR;
    }

    return ArduinoClient::ERROR::SUCCSESS;
}

ArduinoClient::ERROR ArduinoClient::printStatusFromArduino()
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
        return ArduinoClient::ERROR::SERIAL_PORT_ERROR;
    }
    return ArduinoClient::ERROR::SUCCSESS;
}
