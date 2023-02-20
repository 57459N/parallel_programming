cmake_minimum_required(VERSION 3.23)
project(parallel_programming)

set(CMAKE_CXX_STANDARD 23)

include_directories("C:\\Program\ Files\\Microsoft\ MPI"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Lib"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Include")

find_package(MPI REQUIRED)

add_executable(parallel_programming main.cpp 1.1.14.cpp tasks.h)

target_link_libraries(parallel_programming PUBLIC MPI::MPI_CXX)