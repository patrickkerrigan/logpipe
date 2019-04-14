cmake_minimum_required(VERSION 3.13)
project(logpipe)

set(CMAKE_CXX_STANDARD 14)

find_package(aws-sdk-cpp REQUIRED COMPONENTS logs)

add_executable(logpipe main.cpp AwsLogWriter.cpp AwsLogWriter.h EventQueue.cpp EventQueue.h LogReader.cpp LogReader.h LogWriter.h)

target_link_libraries(logpipe aws-cpp-sdk-logs aws-cpp-sdk-core)
