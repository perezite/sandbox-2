#include "MusicImpl.h"
#include "../Logger.h"

// Setting ITERATOR_DEBUG_LEVEL = 0 in debug mode isn't gonny fly with SFML-Audio, so issue a warning if this were the case
#if defined(_ITERATOR_DEBUG_LEVEL) && defined(_DEBUG)
	#if (_ITERATOR_DEBUG_LEVEL == 0)
		#pragma message ( "WARNING: setting _ITERATOR_DEBUG_LEVEL = 0 in debug mode will cause problems with music playback" )
	#endif
#endif

namespace sb
{	
	void MusicImpl::loadFromFile(std::string filePath)
	{
		if (!m_music.openFromFile(filePath))
			SB_ERROR("unable to load music track " << filePath);
	}

	void MusicImpl::play()
	{
		if (m_music.getStatus() == sf::Music::Playing)
			return;

		m_music.play();
	}

	void MusicImpl::stop()
	{
		m_music.stop();
	}

	void MusicImpl::setLooping(bool looping)
	{
		m_music.setLoop(looping);
	}

	void MusicImpl::setVolume(float volume)
	{
		m_music.setVolume(volume * 100.0f);
	}
}
