cmake_minimum_required(VERSION 3.0.0)
project(comunication VERSION 0.1.0)


include_directories("./include")

add_executable(comunication "./src/main.cpp" "./src/serialib.cpp" "./src/arduino.cpp" ".src/")