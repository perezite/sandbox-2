cmake_minimum_required(VERSION 3.13)
project(app)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(calculator)

add_executable(app "main.cpp")

target_link_libraries(app PUBLIC Calculator)