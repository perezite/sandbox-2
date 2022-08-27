#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_organisation_app_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from my C++ App";
    return env->NewStringUTF(hello.c_str());
}