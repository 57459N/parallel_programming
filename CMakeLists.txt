cmake_minimum_required(VERSION 3.23)
project(parallel_programming)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories("C:\\Program\ Files\\Microsoft\ MPI"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Lib"
                    "C:\\Program\ Files\ (x86)\\Microsoft\ SDKs\\MPI\\Include")

find_package(MPI REQUIRED)

add_executable(parallel_programming main.cpp 1.1.14.cpp tasks.h 1.1.5.cpp 1.2.14.cpp 1.3.14.cpp 1.4.14.cpp)

target_link_libraries(parallel_programming PUBLIC MPI::MPI_CXX)