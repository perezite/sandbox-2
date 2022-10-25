cmake_minimum_required(VERSION 3.13) 
project(app)              
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(assimp)

set(CMAKE_EXECUTABLE_SUFFIX .html) 
add_executable(index main.cpp)
set_target_properties(index PROPERTIES LINK_FLAGS "--emrun")

target_link_libraries(index assimp)
