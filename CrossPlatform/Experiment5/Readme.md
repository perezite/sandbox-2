# Content

- Run SDL2 on Android

# Reproduction

- Clone https://github.com/libsdl-org/SDL somewhere
- Read SDL/docs/Readme-android.md
- Copy android-project here
- Copy SDL here (the whole folder)
- In android-project/app/jni
    - Remove all *.mk files (we use CMake instead)
    - Adjust CMakeLists.txt to reference our top-level SDL folder as a "subdirectory"
- In android-project/app/jni/src
    - Add main.cpp with some boilerplate SDL code
    - Reference it in CMakeLists.txt
- In app/src/main/AndroidManifest.xml
    - Replace org.libsdl.app with com.organisation.mygame
    - Replace android:name="SDLActivity" with android:name="MyGame"
- In app/src/main/java
    - Add subfolders: com/organisation/mygame
    - Add com/organisation/mygame/MyGame.java with minimal class
- In app/src/main/res/values/strings
    - Replace Game with Super Duper