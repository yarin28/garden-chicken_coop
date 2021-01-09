#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "serialib.h"
#include <ios>
#include <thread>
#include <functional>

typedef std::function<int(int, float)> DataRciveCallback;

/**
 * @brief the connector between the arduino and the pi
 * it handles the protocol.
 */
class ArduinoClient
{
public:
  enum COMMANDS
  {
    ASK_FOR_STATUS_PER_SENSOR = 30,
    ASK_FOR_OVERALL_STATUS = 300,
  };

  enum MESSAGES
  {
    FLOAT_DATA = 700,
    BOOLEAN = 701,
  };

  enum ERROR
  {
    SUCCSESS = 0,
    SERIAL_PORT_ERROR,
    CHECKSUM_ERROR,
    TIMEOUT_ERROR,
    BAD_STATUS,
    CLOSED_CONNECTION,
  };

  /**
   * @brief Construct a new ArduinoClient object
   * @param portName place to listen to .
   * @param baudRate the rate of serial port .
   * @param timeout the time to exit the serial port.
   * @param dataReivieCallbaack the func
   */
  ArduinoClient(const std::string &portName, unsigned int baudRate, unsigned int timeout, DataRciveCallback dataRciveCallback);

  /*!
  * @brief Destroy the ArduinoClient object 
  */
  ~ArduinoClient();

  /**
   * @brief setter of the timeout
   * 
   * @param timeout int the time to abandon the serial communication in miliseconds
   */
  void setTimeout(int timeout);

  /**
   * @brief getter for the timeout of the serial port
   * 
   * @return int the time to abandon the serial communication in miliseconds
   */
  int getTimout();

  /**
   * @brief check the connection to the arduino and report.
   * @return the error code 
   */
  ArduinoClient::ERROR checkConnection();
  /**
   * @brief this is a wrapper for the message checking.
   * you will need to delete
   */
  std::thread *startTheArduinoCheking();

  /**
   * @brief open the serial port
   * @return ArduinoClient::ERROR will return the error code if there is one. or succsess 
   */
  ArduinoClient::ERROR openSerial();

private:
  /*
  * @brief calculate the sum of a string
  * @param message a string that the checksum will be calcualted from
  * @return int the checksum that was calculated from the message
  */
  int checkSum(const char *message) const;
  /**
   * @brief the simplest way to receive a boolean from the arduino
   * @return bool the value that came from the serial port.
   */
  ArduinoClient::ERROR receiveBoolean(uint8_t &value);
  /**
   * @brief this is the main function,
   * will listen to the serial port and interact with it.
   * 
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  ArduinoClient::ERROR getDataWithWhileLoop();
  /**
   * @brief the function will send the arduino a message to 
   * start checking all of its modules. 
   */

  ArduinoClient::ERROR receiveInt(uint32_t &value);

  /*
  * @brief send a serial commend to the arduino and confirm
  *  that the message has been received 
  * 
  * @param message  the commend to be sent
  * 
  * @return ArduinoClient::ERROR the error code.
  */
  ArduinoClient::ERROR sendSerialCommend(const std::string &message);

  /**
   * @brief will receive a message and write it to the sensor log
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  ArduinoClient::ERROR logFloatFromSensor();
  /**
   * @brief will check the status of the arduino.
   * @return ArduinoClient::ERROR will return the error code if there is one. 
   */
  ArduinoClient::ERROR printStatusFromArduino();
  /**
   * @brief will receive float from serial port
   * 
   * @return float the value that came out.
   */
  ArduinoClient::ERROR receiveFloat(float &value);

  DataRciveCallback dataRciveCallback;
  serialib serial;
  std::string portName;
  unsigned int baudRate;
  unsigned int timeout;
  bool isOpen;
};
