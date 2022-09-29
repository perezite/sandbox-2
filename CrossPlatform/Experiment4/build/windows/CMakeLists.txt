message("Target Platform: Windows")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/windows)

add_subdirectory("../../libraries/calculator" "calculator")

include_directories("../../libraries")

add_subdirectory("../../source" "app")