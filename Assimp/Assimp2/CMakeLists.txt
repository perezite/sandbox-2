cmake_minimum_required(VERSION 3.13) 

project(app)              
set(CMAKE_CXX_STANDARD 14)
# https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/binaries)

#set(BUILD_SHARED_LIBS OFF)
#set(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_NO_EXPORT "Disable Assimp's exporter functionality" ON)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "Disable all of Assimp's importers by default" OFF)
#option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT "Disable all of Assimp's exporters by default" OFF)
option(ASSIMP_BUILD_TESTS "Build tests for Assimp" OFF)
option(ASSIMP_BUILD_ASSIMP_VIEW "Build Assimp viewer" OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)

add_subdirectory(assimp)

set(CMAKE_EXECUTABLE_SUFFIX .html) 
add_executable(index main.cpp)
set_target_properties(index PROPERTIES LINK_FLAGS "-sALLOW_MEMORY_GROWTH --emrun")

target_link_libraries(index assimp)
