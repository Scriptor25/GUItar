file(GLOB_RECURSE src src/*.cpp include/*.hpp include/*.h)
add_library(GUItar STATIC ${src})
target_include_directories(GUItar PUBLIC include)
target_link_libraries(GUItar PUBLIC
        glfw
        libglew_static
        imgui
        implot
)
