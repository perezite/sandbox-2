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
