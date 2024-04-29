cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IMGUI_EXAMPLES OFF)
set(IMGUI_ENABLE_FREETYPE OFF)
set(IMGUI_ENABLE_STDLIB_SUPPORT ON)

add_subdirectory(GLEW)
add_subdirectory(GLFW)
add_subdirectory(ImGui)
add_subdirectory(GUItar)
add_subdirectory(Test)
