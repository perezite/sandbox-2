cmake_minimum_required(VERSION 3.6)

project(MY_APP)

add_subdirectory("../third-party/SDL" "SDL")

add_library(main SHARED main.cpp)

target_link_libraries(main SDL2)


