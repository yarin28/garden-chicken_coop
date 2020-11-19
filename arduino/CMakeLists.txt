cmake_minimum_required(VERSION 3.0.0)
project(Garden VERSION 0.1.0)


include_directories("./include")

add_executable(Garden "./src/main.cpp" "./src/serialib.cpp")


