#include <string>
#include <sstream>
#include <jni.h>
#include <SDL.h>

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) 
{			
	SDL_version version;
	SDL_GetVersion(&version);
	
	char sdlVersion[1024];
	sprintf(sdlVersion, "%u.%u.%u", version.major, version.minor, version.patch);

	ostringstream os;
	os << "Hello from NDK with SDL Version: " << sdlVersion;
	
    return env->NewStringUTF(os.str().c_str());
}