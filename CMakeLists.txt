#state required cmake version
cmake_minimum_required (VERSION 2.6)
#name the project
project (LED)

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")


#command to make LED2.cpp and name it LED
add_executable(LEDMain LEDMain.cpp LED.cpp /home/laborlinux/src/XYTable/XYTable/SourceCode/RS232communication.cpp)











