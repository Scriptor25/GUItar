include(FetchContent)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_Declare(glew
        GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
        GIT_TAG master
)
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_Declare(implot
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG master
)
FetchContent_MakeAvailable(glfw glew imgui implot)

# Setup ImGUI target
FetchContent_GetProperties(imgui)
file(GLOB imgui_src
        ${imgui_SOURCE_DIR}/**.cpp
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${imgui_src})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC glfw)

# Setup ImPlot target
FetchContent_GetProperties(implot)
file(GLOB implot_src ${implot_SOURCE_DIR}/**.cpp)
add_library(implot STATIC ${implot_src})
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

file(GLOB_RECURSE src src/**.cpp include/**.hpp include/**.h)
add_library(GUItar STATIC ${src})
target_include_directories(GUItar PUBLIC include)
target_link_libraries(GUItar PUBLIC
        glfw
        libglew_static
        imgui
        implot
)
