#pragma once
#include "ParticleSystem.h"

namespace blocks {
	class BlockExplosion : public sb::ParticleSystem {
		bool _isActive;

	protected:
		static sb::Texture& getTexture();

	public:
		BlockExplosion(size_t numParticles, char type = 'i');
		inline bool isActive() { return _isActive; }
		void setType(char type);
		virtual void update(float ds);
		void explode();
	};
}