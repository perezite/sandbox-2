cmake_minimum_required(VERSION 3.10)

project(o2-lib)

get_directory_property(O2_PARENT_DIRECTORY PARENT_DIRECTORY)
if (NOT O2_PARENT_DIRECTORY)
	set(O2_STANDALONE TRUE)
endif()

if (O2_STANDALONE)
	# Uncomment, as soon as it's used
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output/binaries)
	# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../output/binaries)
endif()

add_subdirectory(o2-lib)