find_package(OpenGL REQUIRED)

if (UNIX AND NOT APPLE)
    set(GUITAR_LIBS GLdispatch xcb Xau Xdmcp bsd)
endif ()

file(GLOB_RECURSE test_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")
add_executable(Test ${test_src})
target_include_directories(Test PUBLIC include)
target_link_libraries(Test PRIVATE OpenGL::GL GUItar ${GUITAR_LIBS})

install(TARGETS Test)
install(DIRECTORY resources DESTINATION bin)
