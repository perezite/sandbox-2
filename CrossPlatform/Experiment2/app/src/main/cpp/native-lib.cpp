#include <string>
#include <sstream>
#include <jni.h>
#include <SDL.h>

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) 
{			
	ostringstream os;
	os << "Hello from NDK. Was SDL Video initialized: " << SDL_WasInit(SDL_INIT_VIDEO);
	
    return env->NewStringUTF(os.str().c_str());
}