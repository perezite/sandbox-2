add_subdirectory(../../third-party/o2-lib o2-lib)
include_directories(../../third-party/o2-lib/source/o2-lib)

add_executable(app main.cpp)

target_link_libraries(app o2-lib)