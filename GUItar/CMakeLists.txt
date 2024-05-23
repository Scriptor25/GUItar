include(FetchContent)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
)
FetchContent_Declare(glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG master
)
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_MakeAvailable(glfw glew imgui)

FetchContent_GetProperties(imgui)
file(GLOB imgui_src
        ${imgui_SOURCE_DIR}/**.cpp
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${imgui_src})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw)

file(GLOB_RECURSE src src/**.cpp include/**.hpp include/**.h)
add_library(GUItar STATIC ${src})
target_include_directories(GUItar PUBLIC include)
target_link_libraries(GUItar PUBLIC
        glfw
        libglew_static
        imgui
)
