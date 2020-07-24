#include "MusicImpl.h"
#include "AndroidAudio.h"
#include "../Logger.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace sb
{
	MusicImpl::~MusicImpl()
	{
		#ifdef __ANDROID__
			jint result = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "release", "(I)I", m_id);
			SB_WARNING_IF(result == jint(-1), "unable to release music track " << m_assetPath);
		#endif		
	}

	void MusicImpl::loadFromFile(std::string filePath)
	{
		#ifdef __ANDROID__
			if (!AndroidAudio::isInit()) AndroidAudio::init();
			if (!AndroidAudio::isValid()) return;

			m_assetPath = filePath;

			m_id = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newUtfString(filePath));
			SB_WARNING_IF(m_id == jint(-1), "unable to load music track " << filePath);
			
			jint loadResult = 0;
			while (loadResult == jint(0)) {
				loadResult = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "isLoadComplete", "(I)I", m_id);
				SDL_Delay(1);
			}

			SB_WARNING_IF(m_id == jint(-1), "unable to complete loading of music track " << filePath);
		#endif
	}

	void MusicImpl::play()
	{
		#ifdef __ANDROID__
			if (!AndroidAudio::isValid()) return;

			jint result = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "play", "(I)I", m_id);
			SB_WARNING_IF(result == jint(-1), "unable to play music track " << m_assetPath);
		#endif
	}

	void MusicImpl::stop()
	{
		#ifdef __ANDROID__
			if (!AndroidAudio::isValid()) return;

			jint result = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "stop", "(I)I", m_id);
			SB_WARNING_IF(result == jint(-1), "unable to stop music track " << m_assetPath);
		#endif
	}

	void MusicImpl::setLooping(bool looping)
	{
		#ifdef __ANDROID__
			if (!AndroidAudio::isValid()) return;

			jint result = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "setLooping", "(IZ)I", m_id, jboolean(looping));
			SB_WARNING_IF(result == jint(-1), "unable to set looping on music track " << m_assetPath);
		#endif
	}

	void MusicImpl::setVolume(float volume)
	{
		#ifdef __ANDROID__
			if (!AndroidAudio::isValid()) return;

			jint result = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "setVolume", "(IF)I", m_id, jfloat(volume));
			SB_WARNING_IF(result == jint(-1), "unable to set volume on music track " << m_assetPath);
		#endif
	}
}
