find_package(OpenGL REQUIRED)

file(GLOB_RECURSE guitar_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")

add_library(GUItar STATIC ${guitar_src})
target_include_directories(GUItar PUBLIC "include" "../GLEW/include")
target_link_libraries(GUItar PUBLIC glfw glew_s OpenGL::GL)

install(TARGETS GUItar)
