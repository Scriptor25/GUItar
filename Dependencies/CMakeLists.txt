set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(GLFW)

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)
add_subdirectory(GLEW)

file(GLOB imgui_src
        ImGUI/**.cpp
        ImGUI/misc/cpp/imgui_stdlib.cpp
        ImGUI/backends/imgui_impl_glfw.cpp
        ImGUI/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${imgui_src})
target_include_directories(imgui PUBLIC ImGUI)
target_link_libraries(imgui PUBLIC glfw)
