cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

project(bats-pitch)

set(SOURCE_FILES message.h main.cpp vol_calc.h order.h)
add_executable(pitch_parser ${SOURCE_FILES})
install(TARGETS pitch_parser DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/pitch_example_data DESTINATION .)
