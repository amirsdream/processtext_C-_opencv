cmake_minimum_required(VERSION 3.7.2)
project(untitled)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)
#link_directories(
#        /usr/lib
#)
#include_directories(/usr/lib)
add_executable(untitled main.cpp)
target_link_libraries(untitled ${OpenCV_LIBS} tesseract)
