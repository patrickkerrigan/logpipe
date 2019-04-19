cmake_minimum_required(VERSION 3.13)
project(logpipe)

set(CMAKE_CXX_STANDARD 14)

find_package(aws-sdk-cpp REQUIRED COMPONENTS logs)

add_executable(logpipe src/main.cpp src/AwsLogWriter.cpp src/AwsLogWriter.h src/EventQueue.cpp src/EventQueue.h src/LogReader.cpp src/LogReader.h src/LogWriter.h src/ConsoleLogWriter.cpp src/ConsoleLogWriter.h)

target_link_libraries(logpipe aws-cpp-sdk-logs aws-cpp-sdk-core)