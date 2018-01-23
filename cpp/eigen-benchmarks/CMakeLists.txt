cmake_minimum_required(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_INSTALL_PREFIX OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/artifact CACHE PATH "Install prefix" FORCE)
endif()

file(COPY ${CMAKE_SOURCE_DIR}/conanfile.txt DESTINATION .)

execute_process(COMMAND conan install . --settings os=Macos -g cmake --profile gcc RESULT_VARIABLE CONAN_EXIT_CODE)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

set(CMAKE_C_COMPILER   ${CONAN_BIN_DIRS_GCC}/gcc)
set(CMAKE_CXX_COMPILER ${CONAN_BIN_DIRS_GCC}/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(eigen-benchmarks)

conan_basic_setup()

include_directories(${CONAN_INCLUDE_DIRS_GCC})
include_directories(${CONAN_INCLUDE_DIRS_EIGEN})
include_directories(${CONAN_INCLUDE_DIRS_SPDLOG})

set(SOURCE_FILES main.cpp)
add_executable(eigen-benchmarks ${SOURCE_FILES})
target_link_libraries(eigen-benchmarks ${CONAN_LIB_DIRS_GCC}/libstdc++.dylib)
