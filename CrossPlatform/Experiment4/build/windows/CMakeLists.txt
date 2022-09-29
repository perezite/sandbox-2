message("Target Platform: Windows")

add_subdirectory("../../libraries/calculator" "calculator")

include_directories("../../libraries")

add_subdirectory("../../source" "app")