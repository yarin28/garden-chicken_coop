cmake_minimum_required(VERSION 3.0.0)
project(comunication VERSION 0.1.0)

# i need that import for the threads to work propraly 
add_compile_options("-Wall")
include_directories("./include")
# also like line 4
find_package( Threads )
add_executable(comunication "./src/main.cpp" "./src/serialib.cpp" "./src/arduino.cpp" ".src/")
target_link_libraries( comunication ${CMAKE_THREAD_LIBS_INIT} )
