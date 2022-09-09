# Goal
- Incorporate the SDL2 library
- Call GetVersion() from the library and display it on the screen

# Reproduction steps

## Add ABI filters
- I only have SDL2 binaries for x86 and armeabi-v7a readily available. To avoid compilation issues, I am going to restrict the app to compile only for these ABI's
- Read the following, in particular the post at the very bottom: https://stackoverflow.com/questions/32487192/add-abifilters-to-gradle-properties
- Add the abi-filters in build.gradle -> android -> buildTypes -> debug -> ndk -> abiFilters

## Incorporate SDL2 into the build process
- Add a folder app/sdlDistribution with subfolders libs and include
- Drop the libs and include files in some respective subfolders
- Adjust the app/src/main/cpp/CMakeLists.txt file to include the lib and the include files for SDL
- Add some code in native-lib.cpp to show the SDL Version
