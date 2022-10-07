add_library(o2 SHARED version.cpp)

add_subdirectory("../third-party/SDL" "SDL")
include_directories("../third-party/SDL/include")

target_link_libraries(o2 SDL2)