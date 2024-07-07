file(GLOB_RECURSE test_src src/*.cpp include/*.hpp)
add_executable(Test ${test_src})
target_include_directories(Test PRIVATE include)
target_link_libraries(Test PRIVATE GUItar)

install(TARGETS Test)
install(DIRECTORY res DESTINATION bin)
