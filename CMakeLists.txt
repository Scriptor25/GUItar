cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

option(GUITAR_TEST "Enable building the test project" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_LIBRARY_TYPE STATIC)

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)

set(IMGUI_EXAMPLES OFF)
set(IMGUI_ENABLE_FREETYPE ON)
set(IMGUI_ENABLE_STDLIB_SUPPORT ON)

add_subdirectory(GUItar)
if (GUITAR_TEST)
    add_subdirectory(Test)
endif ()
