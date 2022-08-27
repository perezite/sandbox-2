#include <jni.h>
#include <string>
#include "SDL2/SDL.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) 
{
	// TODO: Not working (undefined reference...)
	Uint32 test = SDL_GetTicks();
	
	// This workds, but only because it's a constant
	// SDL_version sdlVersion;
	// SDL_VERSION(&sdlVersion);
	// char sdlVersionBuf[1024];
	// sprintf(sdlVersionBuf, "SDL Version: %u.%u.%u", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);
	
    std::string hello = "Hello from my C++ App";
	// hello += " - " + std::string(sdlVersionBuf);
    return env->NewStringUTF(hello.c_str());
}