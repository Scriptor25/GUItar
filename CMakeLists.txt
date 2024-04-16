cmake_minimum_required(VERSION 3.22.1)

project(GUItar)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE guitar_src "src/guitar/**.cpp" "include/guitar/**.hpp")
file(GLOB_RECURSE imgui_src "src/imgui/**.cpp" "include/imgui/**.h")
file(GLOB_RECURSE test_src "src/test/**.cpp" "include/test/**.hpp")
file(GLOB_RECURSE tinyxml2_src "src/tinyxml2/**.cpp" "include/tinyxml2/**.hpp")

set(GLEW_STATIC 1)

add_library(GUItar ${guitar_src} ${imgui_src} ${tinyxml2_src})
target_include_directories(GUItar PUBLIC "include")
target_link_libraries(GUItar PUBLIC glfw GLEW OpenGL)

install(TARGETS GUItar)

add_executable(Test ${test_src})
target_include_directories(Test PUBLIC "include")
target_link_libraries(Test PUBLIC GUItar)
