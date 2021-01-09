cmake_minimum_required(VERSION 3.0.0)
project(comunication VERSION 0.1.0)
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# i need that import for the threads to work propraly 
add_compile_options("-Wall")
include_directories("./include")
# i need that import for the threads to work propraly 
find_package( Threads )
add_executable(comunication "./src/main.cpp" "./src/serialib.cpp" "./src/arduino.cpp" "./src/FileHandler.cpp")
target_link_libraries( comunication ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(comunication spdlog::spdlog ) 
