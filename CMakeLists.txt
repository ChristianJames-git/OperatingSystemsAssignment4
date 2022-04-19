cmake_minimum_required(VERSION 3.21)
project(OperatingSystemsAssignment4)

set(CMAKE_CXX_STANDARD 11)

add_executable(OperatingSystemsAssignment4 main.cpp producer.cpp producer.h consumer.cpp consumer.h broker.cpp broker.h)
