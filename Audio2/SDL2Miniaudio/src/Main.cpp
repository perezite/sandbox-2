#include "Window.h"
#include "Input.h"
#include "Logger.h"
#include "Asset.h"
#include "Memory.h"
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <exception>

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"    // Enables Vorbis decoding.

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// The stb_vorbis implementation must come after the implementation of miniaudio.
#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

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
	
		// #define fopen(name, mode) sdl2_fopen(name, mode)
	
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

namespace d3
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

	string getAbsoluteFilePath(const string& assetFilePath)
	{
#ifdef WIN32
		return my::combinePath({ my::getExecutableFolderPath(), "../Assets", assetFilePath });
#endif

		return assetFilePath;
	}

	void playSound(const string& path)
	{
		ma_result result = ma_engine_play_sound(&engine, path.c_str(), NULL);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
	}

	static vector<string> soundFilePaths;

	string getRandomSoundFilePath()
	{
		static size_t lastIndex = rand() % soundFilePaths.size();
		size_t currentIndex = lastIndex;

		while (currentIndex == lastIndex)
		{
			currentIndex = rand() % soundFilePaths.size();
		};

		lastIndex = currentIndex;
		return soundFilePaths[currentIndex];
	}

	void initMiniaudio()
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

	void initSoundNames()
	{
		soundFilePaths.push_back("Sounds/killdeer.wav");
		soundFilePaths.push_back("Sounds/ding.flac");
		soundFilePaths.push_back("Sounds/Rotate.mp3");
	}

	void init()
	{
		initMiniaudio();
		initSoundNames();
	}

	void update()
	{
		if (Input::isTouchGoingDown(1))
		{
			string path = getAbsoluteFilePath(getRandomSoundFilePath());
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

namespace d4
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
		SB_MESSAGE("my_vfs_read " << endl);

		SDL_ClearError();
		SDL_RWops* rw = (SDL_RWops*)file;
		size_t result = SDL_RWread(rw, pDst, 1, sizeInBytes);

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

	string getAbsoluteFilePath(const string& assetFilePath)
	{
		#ifdef WIN32
			return my::combinePath({ my::getExecutableFolderPath(), "../Assets", assetFilePath });
		#endif

		return assetFilePath;
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

	void demo()
	{
		Window window;

		init();

		ma_sound sound1;
		ma_sound_init_from_file(&engine, getAbsoluteFilePath("Sounds/Rotate.mp3").c_str(), 0, NULL, NULL, &sound1);
		ma_sound_start(&sound1);

		while (window.isOpen())
		{
			Input::update();
			window.update();

			if (Input::isTouchGoingDown(1) && !ma_sound_is_playing(&sound1))
			{
				ma_sound_start(&sound1);
			}

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}

		ma_sound_uninit(&sound1);
		ma_engine_uninit(&engine);
	}
}

namespace my
{
	string getAbsoluteAssetPath(const string& relativeAssetPath)
	{
		#ifdef WIN32
			return my::combinePath({ my::getExecutableFolderPath(), "../Assets", relativeAssetPath });
		#endif

		return relativeAssetPath;
	}
}

namespace d5
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
		SB_MESSAGE("my_vfs_read " << endl);

		SDL_ClearError();
		SDL_RWops* rw = (SDL_RWops*)file;
		size_t result = SDL_RWread(rw, pDst, 1, sizeInBytes);

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

	void initMiniaudio()
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

	inline void releaseMiniaudio()
	{
		ma_engine_uninit(&engine);
	}

	class Miniaudio {
		Miniaudio() { initMiniaudio(); }
		virtual ~Miniaudio() { releaseMiniaudio(); }
	public:
		static Miniaudio& getInstance() {
			static Miniaudio instance;
			return instance;
		}
		inline static ma_engine& getEngine() { return engine; }
	};
}

namespace d5
{
	void initMiniaudioOnce() {
		Miniaudio::getInstance();
	}

	class Sound {
		ma_sound _sound;
	public:
		Sound(const string& filePath) { 
			initMiniaudioOnce();
			ma_sound_init_from_file(&Miniaudio::getEngine(), filePath.c_str(), 0, NULL, NULL, &_sound);
		}
		virtual ~Sound() { ma_sound_uninit(&_sound); }
		void play() { ma_sound_start(&_sound); }
		bool isPlaying() { return ma_sound_is_playing(&_sound); }
	};

	void demo()
	{
		Window window;
		Sound sound(my::getAbsoluteAssetPath("Sounds/ding.mp3"));

		while (window.isOpen())
		{
			Input::update();
			window.update();

			if (Input::isTouchGoingDown(1) && !sound.isPlaying())
			{
				sound.play();
			}

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace d6
{
	class Sound {
		string _filePath;
		ma_sound _mainSound;
		vector<ma_sound*> _sounds;
	protected:
		void initSound(const string& filePath, ma_sound& sound) {
			ma_sound_init_from_file(&d5::Miniaudio::getEngine(), filePath.c_str(), 0, NULL, NULL, &sound);
		}
		void releasePlaybacks() {
			for (size_t i = 0; i < _sounds.size(); i++)
				ma_sound_uninit(_sounds[i]);
			deleteAll(_sounds);
		}
	public:
		Sound(const string& filePath) : _filePath(filePath) {
			d5::initMiniaudioOnce();
			initSound(_filePath, _mainSound);
		}
		virtual ~Sound() {  
			releasePlaybacks();
			ma_sound_uninit(&_mainSound); 
		}
		void play() { 
			_sounds.push_back(new ma_sound());
			initSound(_filePath, *_sounds.back());
			ma_sound_start(_sounds.back());	
		}
		bool isPlaying() { return ma_sound_is_playing(&_mainSound); }
	};

	void demo()
	{
		Window window;
		Sound sound(my::getAbsoluteAssetPath("Sounds/ding.mp3"));

		while (window.isOpen())
		{
			Input::update();
			window.update();

			if (Input::isTouchGoingDown(1))
				sound.play();

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace my
{
	void sleep(size_t millis) {
		this_thread::sleep_for(chrono::milliseconds(millis));
	}
}

namespace d7
{
	inline void initSound(const string& filePath, ma_sound& sound) {
		ma_sound_init_from_file(&d5::Miniaudio::getEngine(), filePath.c_str(), 0, NULL, NULL, &sound);	
	}
	
	void releaseAndDeleteAllSounds(vector<ma_sound*>& sounds) {
		for_each(sounds.begin(), sounds.end(), ma_sound_uninit);
		deleteAll(sounds);
		sounds.clear();
	}

	void releaseAndDeleteAllFinishedSounds(vector<ma_sound*>& sounds) {
		for (size_t i = 0; i < sounds.size(); i++) {
			if (ma_sound_at_end(sounds[i])) {
				ma_sound_uninit(sounds[i]);
				delete sounds[i];
				sounds[i] = NULL;
			}
		}

		removeAll(sounds, (ma_sound*)NULL);
	}

	class Miniaudio {
		thread _updateThread;
		bool _isThreadRunning;
		mutex soundInstancesMutex;
		vector<ma_sound*> _soundInstances;
		Miniaudio() : _updateThread(&Miniaudio::updateThread, this), _isThreadRunning(true) {
			d5::initMiniaudio(); 
		}
	protected:
		void updateThread() {
			while (_isThreadRunning) {
				my::sleep(500);
				soundInstancesMutex.lock();
				cout << _soundInstances.size() << " ";
				releaseAndDeleteAllFinishedSounds(_soundInstances);
				cout << _soundInstances.size() << endl;
				soundInstancesMutex.unlock();
			}
		}
		void stopUpdateThread() {
			_isThreadRunning = false;
			_updateThread.join();
		}
	public:
		static Miniaudio& getInstance() {
			static Miniaudio instance;
			return instance;
		}
		static void initOnce() { Miniaudio::getInstance(); }
		virtual ~Miniaudio() { 
			stopUpdateThread();
			releaseAndDeleteAllSounds(_soundInstances);
			d5::releaseMiniaudio();
		}
		void playSoundInstance(const string& filePath) {
			soundInstancesMutex.lock();
			_soundInstances.push_back(new ma_sound());
			initSound(filePath, *_soundInstances.back());
			ma_sound_start(_soundInstances.back());
			soundInstancesMutex.unlock();
		}
	};

	class Sound {
		string _filePath;
		ma_sound _mainSound;
	public:
		Sound(const string& filePath) : _filePath(filePath) {
			Miniaudio::initOnce();
			initSound(_filePath, _mainSound);
		}
		virtual ~Sound() { ma_sound_uninit(&_mainSound); }
		void play() { Miniaudio::getInstance().playSoundInstance(_filePath); }
	};

	void demo()
	{
		Window window;
		window.setFramerateLimit(60);
		Sound sound(my::getAbsoluteAssetPath("Sounds/ding.mp3"));

		while (window.isOpen())
		{	
			Input::update();
			window.update();

			if (Input::isTouchGoingDown(1))
				sound.play();

			if (Input::isKeyGoingDown(KeyCode::q))
				break;

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace my {

#ifdef __ANDROID__

	AAssetManager* assetManager = NULL;

	void initializeAndroidAssetManager()
	{
		JNIEnv* jni = getJavaNativeInterface();

		jobject activity = (jobject)SDL_AndroidGetActivity();

		jclass activity_class = jni->GetObjectClass(activity);

		jmethodID activity_class_getAssets = jni->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
		jobject asset_manager = jni->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
		jobject global_asset_manager = jni->NewGlobalRef(asset_manager);

		// AAssetManager* pAssetManager = AAssetManager_fromJava(jni, global_asset_manager);
		assetManager = AAssetManager_fromJava(jni, global_asset_manager);
	}

#endif

}

namespace d8
{
	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	ma_engine engine;
	my_vfs vfs;

#ifdef __ANDROID__

	static int maSeekOriginToWhence(ma_seek_origin maSeekOrigin) 
	{
		if (maSeekOrigin == ma_seek_origin_start)
			return SEEK_SET;
		else if (maSeekOrigin == ma_seek_origin_current)
			return SEEK_CUR;
		else if (maSeekOrigin == ma_seek_origin_end)
			return SEEK_END;
		else {
			SB_ERROR("Invalid Miniaudio seek origin: " << maSeekOrigin);
			return int();
		}
	}

	static ma_result my_vfs_open(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		*pFile = AAssetManager_open(my::assetManager, pFilePath, AASSET_MODE_UNKNOWN);
		return pFile ? MA_SUCCESS : MA_ERROR;
	}

	static ma_result my_vfs_open_w(ma_vfs* pVFS, const wchar_t* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		SB_ERROR("Not implemented!");
		return MA_ERROR;
	}

	static ma_result my_vfs_info(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo)
	{
		pInfo->sizeInBytes = AAsset_getLength((AAsset*)file);
		return MA_SUCCESS;
	}

	static ma_result my_vfs_read(ma_vfs* pVFS, ma_vfs_file file, void* pDst, size_t sizeInBytes, size_t* pBytesRead)
	{
		int result = AAsset_read((AAsset*)file, pDst, sizeInBytes);

		if (pBytesRead != NULL)
			*pBytesRead = result;

		if (result != sizeInBytes)
		{
			if (result == 0)
				return MA_AT_END;
			else if (result < 0)
				return MA_ERROR;
		}

		return MA_SUCCESS;
	}

	static ma_result my_vfs_write(ma_vfs* pVFS, ma_vfs_file file, const void* pSrc, size_t sizeInBytes, size_t* pBytesWritten)
	{
		return MA_ERROR;
	}

	static ma_result my_vfs_seek(ma_vfs* pVFS, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin)
	{
		off_t result = AAsset_seek((AAsset*)file, offset, maSeekOriginToWhence(origin));

		if (result == -1)
			return MA_ERROR;

		return MA_SUCCESS;
	}

	static ma_result my_vfs_tell(ma_vfs* pVFS, ma_vfs_file file, ma_int64* pCursor)
	{
		off_t result = AAsset_seek(((AAsset*)file), 0, SEEK_CUR);
		
		if (result == -1)
			return MA_ERROR;
		
		*pCursor = result;
		return MA_SUCCESS;
	}

	static ma_result my_vfs_close(ma_vfs* pVFS, ma_vfs_file file)
	{
		AAsset_close((AAsset*)file);
		return MA_SUCCESS;
	}

#endif

	void init()
	{
		ma_result result;

#ifdef __ANDROID__
		my::initializeAndroidAssetManager();

		vfs.cb.onOpen = my_vfs_open;
		vfs.cb.onOpenW = my_vfs_open_w;
		vfs.cb.onInfo = my_vfs_info;
		vfs.cb.onRead = my_vfs_read;
		vfs.cb.onWrite = my_vfs_write;
		vfs.cb.onSeek = my_vfs_seek;
		vfs.cb.onTell = my_vfs_tell;
		vfs.cb.onClose = my_vfs_close;

		ma_engine_config config = ma_engine_config_init();
		config.pResourceManagerVFS = &vfs;
		result = ma_engine_init(&config, &engine);
#else
		result = ma_engine_init(NULL, &engine);
#endif

		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");
	}

	void playSound(const string& path)
	{
		ma_result result = ma_engine_play_sound(&engine, path.c_str(), NULL);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to play sound.");
	}

	void playSound()
	{
		static const string path = my::getAbsoluteAssetPath("Sounds/ding.mp3");
		playSound(path);
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
			if (Input::isTouchGoingDown(1))
				playSound();
			window.clear(Color(1, 1, 1, 1));
			window.display();
		}

		ma_engine_uninit(&engine);
	}
}

namespace d9 
{
	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	ma_engine engine;
	my_vfs vfs;

	void initMiniaudio()
	{
		ma_result result;

#ifdef __ANDROID__
		my::initializeAndroidAssetManager();

		vfs.cb.onOpen = d8::my_vfs_open;
		vfs.cb.onOpenW = d8::my_vfs_open_w;
		vfs.cb.onInfo = d8::my_vfs_info;
		vfs.cb.onRead = d8::my_vfs_read;
		vfs.cb.onWrite = d8::my_vfs_write;
		vfs.cb.onSeek = d8::my_vfs_seek;
		vfs.cb.onTell = d8::my_vfs_tell;
		vfs.cb.onClose = d8::my_vfs_close;


		ma_engine_config config = ma_engine_config_init();
		config.pResourceManagerVFS = &vfs;
		result = ma_engine_init(&config, &engine);
#else
		result = ma_engine_init(NULL, &engine);
#endif

		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");
	}

	inline void releaseMiniaudio()
	{
		ma_engine_uninit(&engine);
	}

	inline void initMiniaudioSound(const string& filePath, ma_sound& sound, bool useStreaming = false) {
		ma_uint32 flags = useStreaming ? MA_SOUND_FLAG_STREAM : 0;
		ma_result result = ma_sound_init_from_file(&engine, filePath.c_str(), flags, NULL, NULL, &sound);
		SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize sound.");
	}

	void releaseAndDeleteAllSounds(vector<ma_sound*>& sounds) {
		for_each(sounds.begin(), sounds.end(), ma_sound_uninit);
		deleteAll(sounds);
		sounds.clear();
	}
	
	void releaseAndDeleteAllFinishedSounds(vector<ma_sound*>& sounds) {
		for (size_t i = 0; i < sounds.size(); i++) {
			if (ma_sound_at_end(sounds[i])) {
				ma_sound_uninit(sounds[i]);
				delete sounds[i];
				sounds[i] = NULL;
			}
		}

		removeAll(sounds, (ma_sound*)NULL);
	}

	class Miniaudio {
		thread _updateThread;
		bool _isThreadRunning;
		mutex soundInstancesMutex;
		vector<ma_sound*> _soundInstances;
		Miniaudio() : _updateThread(&Miniaudio::updateThread, this), _isThreadRunning(true) {
			initMiniaudio();
		}
	protected:
		void updateThread() {
			while (_isThreadRunning) {
				my::sleep(500);
				soundInstancesMutex.lock();
				cout << _soundInstances.size() << " ";
				releaseAndDeleteAllFinishedSounds(_soundInstances);
				cout << _soundInstances.size() << endl;
				soundInstancesMutex.unlock();
			}
		}
		void stopUpdateThread() {
			_isThreadRunning = false;
			_updateThread.join();
		}
	public:
		static Miniaudio& getInstance() {
			static Miniaudio instance;
			return instance;
		}
		static void initOnce() { Miniaudio::getInstance(); }
		virtual ~Miniaudio() {
			stopUpdateThread();
			releaseAndDeleteAllSounds(_soundInstances);
			releaseMiniaudio();
		}
		void playSoundInstance(const string& filePath) {
			soundInstancesMutex.lock();
			_soundInstances.push_back(new ma_sound());
			initMiniaudioSound(filePath, *_soundInstances.back());
			ma_sound_start(_soundInstances.back());
			soundInstancesMutex.unlock();
		}
	};

	class MiniaudioSound {
		string _filePath;
		ma_sound _mainSound;
		bool _initialized;
	public:
		MiniaudioSound(const string& filePath, bool useStreaming = false) 
			: _filePath(filePath), _initialized(false) 
		{
			Miniaudio::initOnce();
			initMiniaudioSound(_filePath, _mainSound, useStreaming);
			_initialized = true;
		}
		virtual ~MiniaudioSound() {
			ma_sound_uninit(&_mainSound);
		}
		void play() { Miniaudio::getInstance().playSoundInstance(_filePath); }
	};

	class Sound : public MiniaudioSound {
	public:
		Sound(const string& filePath) : MiniaudioSound(filePath) { }
	};

	void demo()
	{
		Window window;
		window.setFramerateLimit(60);
		Sound sound(my::getAbsoluteAssetPath("Sounds/ding.mp3"));

		while (window.isOpen())
		{
			Input::update();
			window.update();
			window.setFramerateLimit(60);

			if (Input::isTouchGoingDown(1))
				sound.play();

			if (Input::isKeyGoingDown(KeyCode::q))
				break;

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace t0
{
	int counter;

	class Sound {
		size_t _id;
		string _path;
	public:
		Sound(const string& path) : _path(path) {
			_id = counter++;
			cout << "Sound::Sound(" << path << ")" << endl;
		}
		Sound(const Sound& other) : _id(other._id), _path(other._path) { std::cout << "Sound::Copy(" << _id << ")" << std::endl; }
		virtual ~Sound() {
			cout << "Sound::~Sound() (" << _id << ")" << endl;
		}
	};

	void test()
	{
		{
			vector<Sound> sounds;
			//sounds.reserve(2);				// using this prevents reallaction. Destructor is only called once per object then
			sounds.emplace_back("1");
			sounds.emplace_back("2");
			cout << "End of scope" << endl;
		}
		cin.get();
	}
}

namespace my
{
	size_t getNonRepeatingIndex(size_t indexCount, bool debug = false) {
		static size_t lastIndex = 0;

		size_t index = lastIndex;
		while (index == lastIndex)
			index = rand() % indexCount;

		lastIndex = index;
		if (debug)
			cout << index << endl;
		return index;
	}
}

namespace d10
{
	void demo()
	{
		Window window;
		window.setFramerateLimit(60);

		vector<d9::Sound*> sounds;
		sounds.push_back(new d9::Sound(my::getAbsoluteAssetPath("Sounds/killdeer.wav")));
		sounds.push_back(new d9::Sound(my::getAbsoluteAssetPath("Sounds/ding.flac")));
		sounds.push_back(new d9::Sound(my::getAbsoluteAssetPath("Sounds/Rotate.mp3")));

		while (window.isOpen())
		{
			Input::update();
			window.update();
			window.setFramerateLimit(60);

			if (Input::isTouchGoingDown(1)) {
				size_t index = my::getNonRepeatingIndex(sounds.size(), true);
				sounds.at(index)->play();
			}

			if (Input::isKeyGoingDown(KeyCode::q))
				break;

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}

		deleteAll(sounds);
	}
}

namespace d11
{
	class Music : public d9::MiniaudioSound {
	public:
		Music(const string& filePath) : d9::MiniaudioSound(filePath, true) { }
	};

	void demo()
	{
		Window window;
		window.setFramerateLimit(60);

		Music music(my::getAbsoluteAssetPath("Music/BackgroundMusic.ogg"));

		while (window.isOpen())
		{
			Input::update();
			window.update();
			window.setFramerateLimit(60);

			if (Input::isTouchGoingDown(1)) 
				music.play();

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace d12 {
	typedef struct
	{
		ma_vfs_callbacks cb;
	} my_vfs;

	class Miniaudio {
		ma_engine _engine;
		my_vfs _vfs;
		Miniaudio() { 
			ma_result result;

#ifdef __ANDROID__
			my::initializeAndroidAssetManager();

			_vfs.cb.onOpen = d8::my_vfs_open;
			_vfs.cb.onOpenW = d8::my_vfs_open_w;
			_vfs.cb.onInfo = d8::my_vfs_info;
			_vfs.cb.onRead = d8::my_vfs_read;
			_vfs.cb.onWrite = d8::my_vfs_write;
			_vfs.cb.onSeek = d8::my_vfs_seek;
			_vfs.cb.onTell = d8::my_vfs_tell;
			_vfs.cb.onClose = d8::my_vfs_close;

			ma_engine_config config = ma_engine_config_init();
			config.pResourceManagerVFS = &_vfs;
			result = ma_engine_init(&config, &_engine);
#else
			result = ma_engine_init(NULL, &_engine);
#endif

			SB_ERROR_IF(result != MA_SUCCESS, "Failed to initialize audio engine.");
		}
		~Miniaudio() {
			ma_engine_uninit(&_engine);
		}
	public:
		static Miniaudio& getInstance() {
			static Miniaudio instance;
			return instance;
		}
		ma_engine& getEngine() { return _engine; }
	};

	void initMiniaudioOnce() {
		Miniaudio::getInstance();
	}

	class Sound {
		ma_sound _sound;
		string _assetPath;
	public:
		Sound(const string& assetPath): _assetPath(assetPath)
		{ 
			initMiniaudioOnce();
			ma_engine& miniaudioEngine = Miniaudio::getInstance().getEngine();
			ma_sound_init_from_file(&miniaudioEngine, assetPath.c_str(), 0, NULL, NULL, &_sound);
		}
		~Sound() {
			ma_sound_uninit(&_sound);
		}
		inline const string& getAssetPath() const { return _assetPath; }
		inline const bool isPlaying() const { return ma_sound_is_playing(&_sound); }
		void play() { 
			if (ma_sound_is_playing(&_sound))
				ma_sound_seek_to_pcm_frame(&_sound, 0);
			ma_sound_start(&_sound);
		}
	};

	void demo() {
		Window window;
		window.setFramerateLimit(60);

		Sound sound(my::getAbsoluteAssetPath("Sounds/killdeer.wav"));

		while (window.isOpen())
		{
			Input::update();
			window.update();
			window.setFramerateLimit(60);

			if (Input::isTouchGoingDown(1))
				sound.play();

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

namespace d13 {
	d12::Sound* createSound() {
		vector<string> filenames = { "killdeer.wav", "Rotate.mp3", "ding.flac", "Collision.ogg" };
		string path = "Sounds/" + filenames[my::getNonRepeatingIndex(4)];
		return new d12::Sound(my::getAbsoluteAssetPath(path));
	}

	void printSounds(vector<d12::Sound*> sounds) {
#ifdef WIN32
		system("cls");
		for (size_t i = 0; i < sounds.size(); i++) {
			cout << sounds[i]->getAssetPath() << " isPlaying: " << sounds[i]->isPlaying() << endl;
		}
#endif
	}

	static bool hasEnded(d12::Sound* sound) { return !sound->isPlaying(); }

	void demo() {
		Window window;
		vector<d12::Sound*> sounds;
		window.setFramerateLimit(60);
		int counter = -1;

		while (window.isOpen())
		{
			Input::update();
			window.update();
			window.setFramerateLimit(60);

			if (Input::isTouchGoingDown(1)) {
				d12::Sound* sound = createSound();
				sounds.push_back(sound);
				sound->play();
				printSounds(sounds);
			}

			counter++;
			if (counter % 30 == 0) {
				printSounds(sounds);
				deleteAll(sounds, hasEnded);
			}

			window.clear(Color(1, 1, 1, 1));
			window.display();
		}
	}
}

int main() 
{
	//d15::demo();		// TODO: Proper error message when file does not exist
	//d14::demo();		// Every sound instance plays only one sound. Calling play() multiple times causes the sound to restart.
	d13::demo();		// Play multiple sounds 
	//d12::demo();		// New Api: Sound::Play() replays the sound instead of playing a separate sound
	//d11::demo();		// Play music track
	//d10::demo();		// caching, auto cleanup, AAsset, randomized
	//t0::test();		// https://cplusplus.com/forum/general/102593/. Punchline: Always store pointers to complex objects into vectors, not stack objects.
	//d9::demo();		// Play the same cached sound multiple times in parallel with automatic cleanup of old sounds. Use AAsset instead of SDL_RWops
	//d8::demo();		// Play a sound, whenever th user clicks the window. Use native file access (AAsset on Android) instead of SDl_RWops
	//d7::demo();		// Play the same cached sound multiple times in parallel with automatic cleanup of old sounds
	//d6::demo();		// Play the same cached sound multiple times in parallel
	//d5::demo();		// Play a cached sound, using classes
	//d4::demo();		// Play a cached sound
	//d3::demo();		// Play a random sound whenever the user clicks the window
	//d2::demo();		// Play a sound whenever the user clicks the window
	//d1::demo();
	//d0::demo();

	return 0;
}
