cmake_minimum_required(VERSION 3.23)
project(parallel_programming)

set(CMAKE_CXX_STANDARD 23)

include_directories("C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Include"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Lib"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI")

find_package(MPI REQUIRED)

add_executable(parallel_programming main.cpp)
