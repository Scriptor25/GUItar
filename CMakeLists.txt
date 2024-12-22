cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

option(GUITAR_TEST "Enable building the test project" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Dependencies)
add_subdirectory(GUItar)
if (GUITAR_TEST)
    add_subdirectory(Test)
endif ()
