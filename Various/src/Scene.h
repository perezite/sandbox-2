#pragma once
#include "Drawable.h"

namespace sb 
{
	class Scene : public Drawable
	{
	public:
		Scene()
			: m_isPlaying(true)
		{ }

		inline bool isPlaying() const { return m_isPlaying; }

		inline void endScene() { m_isPlaying = false; };

		virtual void update() = 0;
	private: 
		bool m_isPlaying;
	};
}