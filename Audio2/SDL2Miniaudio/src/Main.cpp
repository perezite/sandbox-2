#include "Window.h"
#include "Input.h"

#ifdef __ANDROID__

//#include <jni.h>
//#include <android\asset_manager.h>
//#include <android\asset_manager_jni.h>
//#include "../build/Platform/Android/Application/SDL_android_main.h"
//#include <stdio.h>
//
//void test()
//{
//		JNIEnv* jni = getJavaNativeInterface();
//
//		jobject activity = (jobject)SDL_AndroidGetActivity();
//
//		jclass activity_class = jni->GetObjectClass(activity);
//
//		jmethodID activity_class_getAssets = jni->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
//		jobject asset_manager = jni->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
//		jobject global_asset_manager = jni->NewGlobalRef(asset_manager);
//
//		AAssetManager* pAssetManager = AAssetManager_fromJava(jni, global_asset_manager);
//}

#endif

using namespace sb;

int main() 
{
	Window window;

	while (window.isOpen())
	{
		Input::update();
		window.update();
		window.display();
	}

	return 0;
}