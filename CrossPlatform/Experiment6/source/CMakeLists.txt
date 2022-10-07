add_library(main SHARED main.cpp)

add_subdirectory("../third-party/SDL" "SDL")
include_directories("../third-party/SDL/include")
target_link_libraries(main SDL2)


