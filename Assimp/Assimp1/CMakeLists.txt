cmake_minimum_required(VERSION 3.13) 

project(app)              
set(CMAKE_CXX_STANDARD 14)
# https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

add_subdirectory(assimp)

set(CMAKE_EXECUTABLE_SUFFIX .html) 
add_executable(index main.cpp)
set_target_properties(index PROPERTIES LINK_FLAGS "--emrun")

target_link_libraries(index assimp)
