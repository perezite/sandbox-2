#include <string>
#include <sstream>
#include <jni.h>
#include <gmath.h>
#include <gperf.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) 
{		
    auto ticks = GetTicks();

	unsigned result = 0;
    for (auto exp = 0; exp < 32; ++exp) {
        volatile unsigned val = gpower(exp);
		result = val;
    }
	
    ticks = GetTicks() - ticks;

	std::ostringstream os;
	os << "Result from C++: " << result << ". Calculation took: " << ticks << " ticks"; 
	std::string str = os.str();
    return env->NewStringUTF(str.c_str());
}