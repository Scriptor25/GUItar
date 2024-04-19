file(GLOB_RECURSE test_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")

add_executable(Test ${test_src})
target_include_directories(Test PUBLIC "include")
target_link_libraries(Test PUBLIC GUItar)

install(TARGETS Test)
install(DIRECTORY "res" DESTINATION "bin")
