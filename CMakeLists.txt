cmake_minimum_required(VERSION 3.10)

# Set compiler before project() command
set(CMAKE_CXX_COMPILER "/usr/bin/g++-12")

project(GnomeKeyBinder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use c++2b flag for C++23 with GCC 12
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++2b)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Specify source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    # Add other source files here as needed
)

# Create executable
add_executable(GnomeKeyBinder ${SOURCES})