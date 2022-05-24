#ifdef __ANDROID__
#include "Java.h"
#include "../../build/Platform/Android/Application/SDL_android_main.h"
#include "../Logger.h"
#include <stdarg.h>
#include <jni.h>
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>

namespace {
	void test()
	{
		JNIEnv* jni = getJavaNativeInterface();

		jobject activity = (jobject)SDL_AndroidGetActivity();

		jclass activity_class = jni->GetObjectClass(activity);

		jmethodID activity_class_getAssets = jni->GetMethodID(activity_class, "getAssets", "()Landroid/content/res/AssetManager;");
		jobject asset_manager = jni->CallObjectMethod(activity, activity_class_getAssets); // activity.getAssets();
		jobject global_asset_manager = jni->NewGlobalRef(asset_manager);

		AAssetManager* pAssetManager = AAssetManager_fromJava(jni, global_asset_manager);
	}
}

namespace sb
{
	std::map<std::string, jclass> Java::m_classes;
	std::map<JavaMethod, jmethodID> Java::m_methods;

	jint Java::callStaticIntMethod(std::string classDescriptor, std::string methodName, std::string methodDescriptor, ...)
	{
		static JNIEnv* jni = getJavaNativeInterface();

		if (m_classes.find(classDescriptor) == m_classes.end())
			loadClass(classDescriptor);

		JavaMethod method { m_classes[classDescriptor], methodName, methodDescriptor };
		if (m_methods.find(method) == m_methods.end())
			loadStaticMethod(method);

		va_list args;
		va_start(args, methodDescriptor);
			jint result = jni->CallStaticIntMethodV(m_classes[classDescriptor], m_methods[method], args);
		va_end(args);	

		return result;
	}

	jstring Java::newUtfString(std::string string) {
		static JNIEnv* jni = getJavaNativeInterface();
		return jni->NewStringUTF(string.c_str());
	}

	void Java::loadClass(std::string classDescriptor)
	{
		static JNIEnv* jni = getJavaNativeInterface();
		jclass theClass = jni->FindClass(classDescriptor.c_str());
		SB_ERROR_IF(theClass == NULL, "unable to load java android class with descriptor " << classDescriptor);

		m_classes[classDescriptor] = theClass;
	}

	void Java::loadStaticMethod(JavaMethod javaMethod)
	{
		static JNIEnv* jni = getJavaNativeInterface();
		jmethodID methodId = jni->GetStaticMethodID(javaMethod.theClass, javaMethod.name.c_str(), javaMethod.descriptor.c_str());
		SB_ERROR_IF(methodId == NULL, "unable to load java android method " << javaMethod.name);

		m_methods[javaMethod] = methodId;
	}
}

#endif