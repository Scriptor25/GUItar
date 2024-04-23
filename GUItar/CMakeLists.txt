find_package(OpenGL REQUIRED)

file(GLOB_RECURSE guitar_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")

get_target_property(GLEW_INCLUDE_DIRS glew_s INCLUDE_DIRECTORIES)

add_library(GUItar STATIC ${guitar_src})
target_include_directories(GUItar PUBLIC "include" ${GLEW_INCLUDE_DIRS})
target_link_libraries(GUItar PUBLIC glfw glew_s OpenGL::GL)
