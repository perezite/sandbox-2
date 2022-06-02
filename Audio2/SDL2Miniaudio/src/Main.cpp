#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Asset.h"
#include <string>
#include <algorithm>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#ifdef __ANDROID__
	#include "../build/Platform/Android/Application/SDL_android_main.h"
	#include <stdio.h>
	#include <jni.h>
	#include <android\asset_manager.h>
	#include <android\asset_manager_jni.h>
#endif

using namespace std;
using namespace sb;

namespace d0
{
	// fopen sdl2 hack
	#ifdef __ANDROID__

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
			return 0;
			//return SDL_RWclose((SDL_RWops*)cookie);
		}

		FILE* sdl2_fopen(const char* fname, const char* mode)
		{
			/*	SDL_RWops* reader = SDL_RWFromFile(fname, mode);
				auto result = funopen(reader, android_read, android_write, android_seek, android_close);
				auto test = fileno(result);
				return result;*/

			AAsset* asset = AAssetManager_open(g_AssetManager, fname, 0);

			// off_t length = AAsset_getLength(asset);

			auto result = funopen(asset, android_read, android_write, android_seek, android_close);

			//auto test = fileno(result);

			return result;
		}
	
		#define fopen(name, mode) sdl2_fopen(name, mode)
	
	#endif

	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	ma_engine engine;
	my_vfs vfs;

	static ma_result my_vfs_open(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		string mode;
		if ((openMode & MA_OPEN_MODE_READ) != 0) {
			if ((openMode & MA_OPEN_MODE_WRITE) != 0)
				mode = "r+";
			else
				mode = "rb";
		}
		else
			mode = "wb";

		SDL_RWops* handle = SDL_RWFromFile(pFilePath, mode.c_str());
		ma_result result = handle ? MA_SUCCESS : MA_ERROR;

		*pFile = handle;
		return result;
	}

	static ma_result my_vfs_info(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo)
	{
		SDL_RWops* rw = (SDL_RWops*)file;
		pInfo->sizeInBytes = SDL_RWsize(rw);

		return MA_SUCCESS;
	}

	static ma_result my_vfs_read(ma_vfs* pVFS, ma_vfs_file file, void* pDst, size_t sizeInBytes, size_t* pBytesRead)
	{
		size_t result;

		SDL_ClearError();
		SDL_RWops* rw = (SDL_RWops*)file;
		result = SDL_RWread(rw, pDst, 1, sizeInBytes);

		if (pBytesRead != NULL)
			*pBytesRead = result;

		if (result == 0)
		{
			const char* check = SDL_GetError();
			bool hasError = check != NULL && strlen(check) > 0;
			return hasError ? MA_ERROR : MA_AT_END;
		}

		return MA_SUCCESS;
	}

	static ma_result my_vfs_close(ma_vfs* pVFS, ma_vfs_file file)
	{
		SDL_RWclose((SDL_RWops*)file);

		return MA_SUCCESS;
	}


	void playSound(const string& path)
	{
		vfs.cb.onOpen = my_vfs_open;
		vfs.cb.onInfo = my_vfs_info;
		vfs.cb.onRead = my_vfs_read;
		vfs.cb.onClose = my_vfs_close;

		ma_result result;
		ma_engine_config config = ma_engine_config_init();
		config.pResourceManagerVFS = &vfs;
		result = ma_engine_init(&config, &engine);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");

		result = ma_engine_play_sound(&engine, path.c_str(), NULL);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
	}

	void demo()
	{
		Window window;

		#ifdef __ANDROID__
			initializeAndroidAssetManager();
			sdl2_fopen("Sounds/killdeer.wav", "rb");
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
	}
}

namespace d1
{
	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	ma_engine engine;
	my_vfs vfs;

	static ma_result my_vfs_open(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		string mode;
		if ((openMode & MA_OPEN_MODE_READ) != 0) {
			if ((openMode & MA_OPEN_MODE_WRITE) != 0)
				mode = "r+";
			else
				mode = "rb";
		}
		else
			mode = "wb";

		SDL_RWops* rw = SDL_RWFromFile(pFilePath, mode.c_str());
		ma_result result = rw ? MA_SUCCESS : MA_ERROR;

		*pFile = rw;
		return result;
	}

	static ma_result my_vfs_info(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo)
	{
		SDL_RWops* rw = (SDL_RWops*)file;
		pInfo->sizeInBytes = SDL_RWsize(rw);

		return MA_SUCCESS;
	}

	static ma_result my_vfs_read(ma_vfs* pVFS, ma_vfs_file file, void* pDst, size_t sizeInBytes, size_t* pBytesRead)
	{
		size_t result;

		SDL_ClearError();
		SDL_RWops* rw = (SDL_RWops*)file;
		result = SDL_RWread(rw, pDst, 1, sizeInBytes);

		if (pBytesRead != NULL)
			*pBytesRead = result;

		if (result == 0)
		{
			const char* check = SDL_GetError();
			bool hasError = check != NULL && strlen(check) > 0;
			return hasError ? MA_ERROR : MA_AT_END;
		}

		return MA_SUCCESS;
	}

	static ma_result my_vfs_close(ma_vfs* pVFS, ma_vfs_file file)
	{
		SDL_RWclose((SDL_RWops*)file);
		return MA_SUCCESS;
	}


	void playSound(const string& path)
	{
		vfs.cb.onOpen = my_vfs_open;
		vfs.cb.onInfo = my_vfs_info;
		vfs.cb.onRead = my_vfs_read;
		vfs.cb.onClose = my_vfs_close;

		ma_result result;
		ma_engine_config config = ma_engine_config_init();
		config.pResourceManagerVFS = &vfs;
		result = ma_engine_init(&config, &engine);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");

		result = ma_engine_play_sound(&engine, path.c_str(), NULL);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
	}

	void demo()
	{
		Window window;

		#ifdef __ANDROID__
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
	}
}

namespace my
{
	const char PathSeparator = '/';

	string combinePath(vector<string> parts)
	{
		string result;
		for (unsigned int i = 0; i < parts.size() - 1; i++)
		{
			if (parts[i].size() > 0)
			{
				result += parts[i];
				result += PathSeparator;
			}
		}

		result += parts[parts.size() - 1];

		return result;
	}

	#ifdef WIN32	
		void convertWideStringToMultibyteString(const std::wstring& wideString, std::string& result)
		{
			// disable visual studio warning about wcstombs being deprecated
			#pragma warning( push )
			#pragma warning( disable : 4996 )

			// determine the length of the resulting multibyte string
			size_t multibyteLength = wcstombs(NULL, wideString.c_str(), 0);

			// convert wide string to multibyte string
			char* multibyteResult = new char[multibyteLength + 1];		// one extra byte for null-termination character
			wcstombs(multibyteResult, wideString.c_str(), multibyteLength + 1);
			result.assign(multibyteResult);

			delete[] multibyteResult;

			#pragma warning ( pop )
		}
	#endif
	
	#ifdef WIN32
		string getExecutableFolderPath()
		{
			wchar_t filePath[MAX_PATH];

			// get executable file path
			HMODULE hModule = GetModuleHandle(NULL);
			SB_ERROR_IF(hModule == NULL, "GetModuleHandle() failed");
			GetModuleFileName(hModule, filePath, (sizeof(filePath)));

			// get file path as string
			wstring filePathAsWideString(filePath);
			string filePathAsString;
			convertWideStringToMultibyteString(filePathAsWideString, filePathAsString);

			// remove trailing file name
			size_t lastPathSeparatorPosition = filePathAsString.rfind('\\');
			string directoryPathAsString = filePathAsString.substr(0, lastPathSeparatorPosition);

			replace(directoryPathAsString.begin(), directoryPathAsString.end(), '\\', PathSeparator);

			return directoryPathAsString;
		}
	#endif

	string getAssetFolderPath()
	{
		#ifdef WIN32
			return combinePath({ getExecutableFolderPath(), "..", "Assets" });
		#elif defined(__ANDROID__)
			return "";
		#else
			#error os not supported
		#endif	
	}
}

namespace d2
{
	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	ma_engine engine;
	my_vfs vfs;

	static ma_result my_vfs_open(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		string mode;
		if ((openMode & MA_OPEN_MODE_READ) != 0) {
			if ((openMode & MA_OPEN_MODE_WRITE) != 0)
				mode = "r+";
			else
				mode = "rb";
		}
		else
			mode = "wb";

		SDL_RWops* rw = SDL_RWFromFile(pFilePath, mode.c_str());
		ma_result result = rw ? MA_SUCCESS : MA_ERROR;

		*pFile = rw;
		return result;
	}

	static ma_result my_vfs_info(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo)
	{
		SDL_RWops* rw = (SDL_RWops*)file;
		pInfo->sizeInBytes = SDL_RWsize(rw);

		return MA_SUCCESS;
	}

	static ma_result my_vfs_read(ma_vfs* pVFS, ma_vfs_file file, void* pDst, size_t sizeInBytes, size_t* pBytesRead)
	{
		size_t result;

		SDL_ClearError();
		SDL_RWops* rw = (SDL_RWops*)file;
		result = SDL_RWread(rw, pDst, 1, sizeInBytes);

		if (pBytesRead != NULL)
			*pBytesRead = result;

		if (result == 0)
		{
			const char* check = SDL_GetError();
			bool hasError = check != NULL && strlen(check) > 0;
			return hasError ? MA_ERROR : MA_AT_END;
		}

		return MA_SUCCESS;
	}

	static ma_result my_vfs_close(ma_vfs* pVFS, ma_vfs_file file)
	{
		SDL_RWclose((SDL_RWops*)file);
		return MA_SUCCESS;
	}

	void init()
	{
		vfs.cb.onOpen = my_vfs_open;
		vfs.cb.onInfo = my_vfs_info;
		vfs.cb.onRead = my_vfs_read;
		vfs.cb.onClose = my_vfs_close;

		ma_result result;
		ma_engine_config config = ma_engine_config_init();
		config.pResourceManagerVFS = &vfs;
		result = ma_engine_init(&config, &engine);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");
	}

	string getAbsoluteFilePath(const string& assetFilePath)
	{
		#ifdef WIN32
			return my::combinePath({ my::getExecutableFolderPath(), "../Assets", assetFilePath});
		#endif

		return assetFilePath;
	}

	void playSound(const string& path)
	{
		ma_result result = ma_engine_play_sound(&engine, path.c_str(), NULL);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
	}

	void update()
	{
		if (Input::isTouchGoingDown(1))
		{
			static const string path = getAbsoluteFilePath("Sounds/killdeer.wav");
			playSound(path);
		}
	}

	void demo()
	{
		ma_engine engine;
		Window window;

		init();

		while (window.isOpen())
		{
			Input::update();
			window.update();
			update();
			window.clear(Color(1, 1, 1, 1));
			window.display();
		}

		ma_engine_uninit(&engine);
	}
}

int main() 
{
	d2::demo();
	//d1::demo();
	//d0::demo();

	return 0;
}