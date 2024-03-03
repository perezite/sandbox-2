# Fügen Sie hier die Quelldateien Ihrer Anwendung hinzu
add_executable(MeineApp main.cpp )

# Fügen Sie das Verzeichnis für die externen Bibliotheken hinzu
target_include_directories(MeineApp PRIVATE ${CMAKE_SOURCE_DIR}/third-party/profiler/src)

# Verknüpfen Sie die Anwendung mit der Profiler-Bibliothek
target_link_libraries(MeineApp PRIVATE profiler)