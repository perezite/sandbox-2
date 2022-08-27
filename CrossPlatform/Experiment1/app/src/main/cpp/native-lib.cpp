#include <jni.h>
#include <string>
#include "SDL2/SDL.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) 
{
	SDL_version sdlVersion;
	SDL_VERSION(&sdlVersion);
	char sdlVersionBuf[1024];
	sprintf(sdlVersionBuf, "SDL Version: %u.%u.%u", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	
    std::string hello = "Hello from my C++ App";
	hello += " - " + std::string(sdlVersionBuf);
    return env->NewStringUTF(hello.c_str());
}