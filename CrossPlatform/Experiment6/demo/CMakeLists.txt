add_subdirectory("../library" "o2")
include_directories("../library")

if(O2_ANDROID)
	add_library(main SHARED "main.cpp")
endif()

target_link_libraries(main o2)
