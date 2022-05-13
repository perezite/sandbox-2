# Android file access for miniaudio

How to access data in the assets folder of the apk?

## Method 1
- We defined a method called android_fopen, which calls funopen(cookie, android_read, android_write, android_seek, android_close) internally to redirect read/write/seek/close calls to designated functions
- the designated functions then use the AAssetManager (or in our case SDL_RWOps) to do the file operations
- We can use a macro to redirect any fopen calls to android_fopen 
- Since miniaudio is a header-only library, we just have to call the redirect-macro before we include miniaudio. Then, miniaudio will use our injected fopen() method internally. 
- This can probably used for other libraries as well, even if they are not header-only libraries. They have to be rebuilt in this case though (see article below)
- To get the AssetManager, we can use the following:  
	```
	#include "../build/Platform/Android/Application/SDL_android_main.h"	// contains the method getJavaNativeInterface()
	#include <jni.h>
	#include <android\asset_manager.h>
	#include <android\asset_manager_jni.h>
	#include <stdio.h>

	void test()
	{
			JNIEnv* jni = getJavaNativeInterface();

			jobject activity = (jobject)SDL_AndroidGetActivity();

			jclass activity_class = jni->GetObjectClass(activity);

			jmethodID activity_class_getAssets = jni->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
			jobject asset_manager = jni->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
			jobject global_asset_manager = jni->NewGlobalRef(asset_manager);

			AAssetManager* pAssetManager = AAssetManager_fromJava(jni, global_asset_manager);
	}

	```
- Sources
	- http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/#comment-1850768990
	- https://devdreamz.com/question/380437-android-ndk-why-is-aassetmanager-open-returning-null
	- https://stackoverflow.com/questions/18090483/fopen-fread-apk-assets-from-nativeactivity-on-android
	- Raylib Audio uses this trick as well

## Method 2: Miniaudio virtual file system
- Mini audio has a virtual file system (VFS) support. It's used as follows: Engine -> Asset Manager -> Virtual File System
- The VFS feature is basically not documented. You have to read the code (it's reasonably easy to understand though)
- The VFS could probably be used to load files using SDL_RWOps

