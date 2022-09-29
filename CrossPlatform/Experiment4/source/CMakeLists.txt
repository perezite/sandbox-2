add_executable(app "main.cpp")

add_subdirectory("../libraries/calculator" "calculator")

include_directories("../libraries")

target_link_libraries(app PUBLIC calculator)