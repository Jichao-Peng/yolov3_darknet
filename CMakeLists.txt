cmake_minimum_required(VERSION 3.7)
project(yolov3)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV)

set(SOURCE_FILES src/yolov3.cpp src/yolov3.h)

add_executable(yolov3 ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(/usr/lib/python2.7/config-i386-linux-gnu)

target_link_libraries(yolov3 python2.7 ${OpenCV_LIBS})