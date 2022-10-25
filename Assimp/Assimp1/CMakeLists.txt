cmake_minimum_required(VERSION 3.13) 
project(app)              
set(CMAKE_CXX_STANDARD 14)         
set(CMAKE_EXECUTABLE_SUFFIX .html)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
 
add_executable(index main.cpp)
set_target_properties(
    index
    PROPERTIES
    LINK_FLAGS
    "--emrun"
)