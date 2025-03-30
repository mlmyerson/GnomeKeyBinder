cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "/usr/bin/g++-12")

project(GnomeKeyBinder)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use c++2b flag for C++23 with GCC 12
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++2b)
endif()

# make main project
include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

add_executable(GnomeKeyBinder ${SOURCES})

# make unit tests
find_package(Boost 1.65 REQUIRED COMPONENTS unit_test_framework)


if(Boost_FOUND)

    set(TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/tests/main_test.cpp
    )
    add_executable(GnomeKeyBinderTest ${TEST_SOURCES})

    target_include_directories(GnomeKeyBinderTest PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(GnomeKeyBinderTest PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

else()
    message(FATAL_ERROR "Boost not found!")
endif()

