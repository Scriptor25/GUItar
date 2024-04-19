cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

set(GLEW_STATIC 1)

add_subdirectory(GUItar)
add_subdirectory(Test)
