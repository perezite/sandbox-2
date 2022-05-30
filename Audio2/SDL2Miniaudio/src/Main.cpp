#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Asset.h"

// fopen sdl2 hack
#ifdef __ANDROID__

	#include <jni.h>
	#include <android\asset_manager.h>
	#include <android\asset_manager_jni.h>
	#include "../build/Platform/Android/Application/SDL_android_main.h"
	#include <stdio.h>

	AAssetManager* g_AssetManager = NULL;

	void initializeAndroidAssetManager()
	{
		JNIEnv* jni = getJavaNativeInterface();

		jobject activity = (jobject)SDL_AndroidGetActivity();

		jclass activity_class = jni->GetObjectClass(activity);

		jmethodID activity_class_getAssets = jni->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
		jobject asset_manager = jni->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
		jobject global_asset_manager = jni->NewGlobalRef(asset_manager);

		// AAssetManager* pAssetManager = AAssetManager_fromJava(jni, global_asset_manager);
		g_AssetManager = AAssetManager_fromJava(jni, global_asset_manager);

	}

	static int android_read(void* cookie, char* buf, int size) {
		return AAsset_read((AAsset*)cookie, buf, size);
		// return SDL_RWread((SDL_RWops*)cookie, buf, sizeof(char), size);
	}

	static int android_write(void* cookie, const char* buf, int size) {
		return EACCES; // can't provide write access to the apk
		//return SDL_RWwrite((SDL_RWops*)cookie, buf, sizeof(char), size);
	}

	static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
		return AAsset_seek((AAsset*)cookie, offset, whence);
		//return SDL_RWseek((SDL_RWops*)cookie, offset, whence);
	}

	static int android_close(void* cookie) {
		AAsset_close((AAsset*)cookie);
		//return SDL_RWclose((SDL_RWops*)cookie);
	}

	FILE* sdl2_fopen(const char* fname, const char* mode)
	{
	/*	SDL_RWops* reader = SDL_RWFromFile(fname, mode);
		auto result = funopen(reader, android_read, android_write, android_seek, android_close);
		auto test = fileno(result);
		return result;*/

		AAsset* asset = AAssetManager_open(g_AssetManager, fname, 0);
		
		auto result = funopen(asset, android_read, android_write, android_seek, android_close);
		
		auto test = fileno(result);

		return result;
	}

	#define fopen(name, mode) sdl2_fopen(name, mode)

#endif

// include miniaudio
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>

using namespace std;
using namespace sb;

ma_engine engine;

void playSound(const string& path)
{
    ma_result result;

    result = ma_engine_init(NULL, &engine);
    SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");

    result = ma_engine_play_sound(&engine, path.c_str(), NULL);
	SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
}

int main() 
{
	Window window;

	#ifdef __ANDROID__
		initializeAndroidAssetManager();
		playSound("Sounds/killdeer.wav");
	#else
		playSound("C:\\Data\\Indie\\Development\\sandbox-2\\Audio2\\SDL2Miniaudio\\bin\\Assets\\Sounds\\killdeer.wav");
	#endif


	while (window.isOpen())
	{
		Input::update();
		window.update();
		window.clear(Color(1, 1, 1, 1));
		window.display();
	}

	ma_engine_uninit(&engine);
	return 0;
}