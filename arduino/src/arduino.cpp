#include "arduino.h"

Arduino::Arduino(){
    Arduino::portName = "/dev/ttyUSB0";
    Arduino::buadRate =9600; 
    Arduino::timeout = 10000;

}

Arduino::Arduino(std::string fileToWrite,std::string portName, unsigned int buadRate, unsigned int timeout){
    Arduino::fileToWrite.open(fileToWrite.c_str());

}
int Arduino::checkConnection(){
    char helper = Arduino::serial.openDevice(portName.c_str(),buadRate);
     if (helper != 1){
        serial.closeDevice();
        return helper;}
    serial.closeDevice();
    return helper;
}
void Arduino::checkConnectionToConsule(){
  int status = checkConnection();
  if (status!=1){
    std::cout << "there was a problem with the connection, the problen code is- " << status << std::endl;
  }
  
    std::cout << "the connection is successful!-" << status <<"-"<< std::endl;
}