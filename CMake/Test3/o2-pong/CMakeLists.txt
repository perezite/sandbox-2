cmake_minimum_required(VERSION 3.10)

project(o2-pong)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/binaries)

add_subdirectory(third-party/test-submodule lib)
include_directories(third-party/test-submodule/source/lib)

add_executable(app "source/o2-pong/main.cpp")

target_link_libraries(app lib)