cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 17)

file(COPY ${CMAKE_SOURCE_DIR}/conanfile.txt DESTINATION .)

execute_process(COMMAND conan install . --settings os=Macos -g cmake --profile gcc RESULT_VARIABLE CONAN_EXIT_CODE)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (APPLE)
    set(GCC_ROOT_DIR /usr/local/bin)

    set(CMAKE_C_COMPILER   ${GCC_ROOT_DIR}/gcc-7)
    set(CMAKE_CXX_COMPILER ${GCC_ROOT_DIR}/g++-7)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

project(eigen-benchmarks)

conan_define_targets()

include_directories(${CONAN_INCLUDE_DIRS_EIGEN})
include_directories(${CONAN_INCLUDE_DIRS_SPDLOG})

set(SOURCE_FILES main.cpp)
add_executable(eigen-benchmarks ${SOURCE_FILES})
install(TARGETS eigen-benchmarks DESTINATION ${CMAKE_CURRENT_BINARY_DIR})