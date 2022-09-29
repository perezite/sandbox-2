set(cmake_cxx_standard 11)
set(cmake_cxx_standard_required true)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_subdirectory("windows")
endif()