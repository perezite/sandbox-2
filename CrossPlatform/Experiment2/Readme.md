# What this does
- Get hello-libs NDK working using the latest and greatest Android Studio project
- Verify that deployment via gradle works

# Reproduction steps

## Setup basic app 
### Generate the App Code
- Android Studio -> New Project -> Native C++ 
- Add the following settings
	- Name: App
	- Package Name: com.example.app
	- Save Location: Somewhere
	- C++ Standard: C++ 11
- Wait for the gradle build to complete

### Copy the App folder to this folder here
### Verify that building from command line works
- cd to this folder here
- gradlew clean
- gradlew assembleDebug
- gradlew clean

### Read the following articles
- https://developer.android.com/studio/build/build-variants
- https://developer.android.com/studio/build
- https://developer.android.com/studio/releases/gradle-plugin#updating-plugin
- https://developer.android.com/studio/build/dependencies

### Verify that deploying the app works
- Open Android Emulator from VS
- gradlew installDebug
- Run the app

## Try to link SDL2 (incomplete)

### First try: Add SDL2 via jniLibs folder
- Basic idea was to add the SDL2 *.so files to jniLibs folder
	- Libs in jniLibs folder are included into the apk automatically
- Then i added the include folder of SDL2 directly netxt to the native-lib.cpp file
- Then i tried calling SDL_GetVersion, giving me a linker error!

### Second try: Add SDL2 in CMakeLists.txt
- I looked up how they included libraries in the NDK example:
	https://github.com/android/ndk-samples/tree/main/hello-libs
- So, I added the SDL2 shared library files to a dedicated distribution folder and linked to it in CMakeLists
- Using this, the *.so files get included in the apk, even though they are not in the jniLibs folder
- Unfortunately, I still receive linker errors when trying to actually call a method

### Things to note
- 'Fake' *.so files 
	- I copied the SDL2 *.so files from another project. However, this project had no ABI's arm64-v8a and x86_64.
	- This caused compiler errors. Just copying another *.so file would not work either because the compiler cannot read it if it's in the wrong ABI.
	- So I just took some existing *.so files from [here](https://github.com/android/ndk-samples/tree/main/hello-libs/distribution/gperf/lib) for arm64_v8 and x86_64 and renamed these to libSDL2.so. I did this to satisfy the compiler / linker. Running the app with these ABI's may cause a crash.
	
### Next steps
- Use hello-libs NDK sample as a starting point
	- This example seems to have just what I need
	- https://github.com/android/ndk-samples/tree/main/hello-libs
	- Incorporate the hello-libs example into the existing structure. 
	- I will then have the working library-call from the hello-libs example along-side the SDL2 library-call causing a linker error.
	- I can then compare the two and find out, why the SDL2 library-call is not working

# Sources
