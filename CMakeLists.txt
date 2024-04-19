cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(GLFW)
add_subdirectory(GLEW/build/cmake)
add_subdirectory(GUItar)
add_subdirectory(Test)
