cmake_minimum_required(VERSION 3.8)
project(dz1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp BigDecimal.cpp BigDecimal.h)
add_executable(dz1 ${SOURCE_FILES})