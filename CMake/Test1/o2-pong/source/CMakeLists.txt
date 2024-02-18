cmake_minimum_required(VERSION 3.10)

project(o2-pong)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output/binaries)

add_subdirectory(../third-party/o2-lib o2-lib)
include_directories(../third-party/o2-lib/source/o2-lib)

add_executable(app "o2-pong/main.cpp")

target_link_libraries(app o2-lib)