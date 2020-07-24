#include "BlockExplosion.h"
#include "Block.h"

namespace blocks {
	sb::Texture& BlockExplosion::getTexture() {
		static sb::Texture texture("Textures/Particle1.png");
		return texture;
	}

	BlockExplosion::BlockExplosion(size_t numParticles, char type) : ParticleSystem(numParticles), _isActive(false)
	{
		setLifetime(1);
		setEmissionRatePerSecond(0);
		setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 1));
		setParticleSpeedRange(sb::Vector2f(0.1f, 3));
		setEmissionType(sb::ParticleSystem::EmissionType::Random);
		setParticleSizeRange(sb::Vector2f(0.01f, 0.25f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		getTexture().enableMipmap(true);
		setTexture(getTexture());
		setType(type);
		addBurst(0, 50);
	}

	void BlockExplosion::setType(char type) {
		type = tolower(type);
		setParticleColor(Block::getColor(type));
	}

	void BlockExplosion::update(float ds) {
		if (_isActive) {
			ParticleSystem::update(ds);
			if (!isPlaying())
				_isActive = false;
		}
	}

	void BlockExplosion::explode() {
		if (!_isActive) {
			reset();
			_isActive = true;
		}
	}
}