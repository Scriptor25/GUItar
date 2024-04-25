file(GLOB_RECURSE test_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")

if (UNIX AND NOT APPLE)
    set(GUITAR_LIBS GLdispatch xcb Xau Xdmcp bsd)
endif ()

add_executable(Test ${test_src})
target_include_directories(Test PUBLIC "include")
target_link_libraries(Test PRIVATE GUItar ${GUITAR_LIBS})

install(TARGETS Test)
install(DIRECTORY "res" DESTINATION "bin")
