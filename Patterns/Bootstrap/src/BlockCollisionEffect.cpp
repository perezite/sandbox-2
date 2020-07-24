#include "BlockCollisionEffect.h"
#include "Box.h"

namespace blocks {
	sb::Texture& BlockCollisionEffect::getTexture() {
		static sb::Texture texture("Textures/Particle1.png");
		return texture;
	}

	void BlockCollisionEffect::setDirection(Position direction) {
		if (direction == Position::Left) {
			setPosition(-0.5f, 0);
			setRotation(-90 * sb::ToRadian);
		}
		else if (direction == Position::Top) {
			setPosition(0, 0.5f);
			setRotation(0 * sb::ToRadian);
		}
		else if (direction == Position::Right) {
			setPosition(0.5f, 0);
			setRotation(90 * sb::ToRadian);
		}
		else if (direction == Position::Bottom) {
			setPosition(0, -0.5f);
			setRotation(180 * sb::ToRadian);
		}
	}

	BlockCollisionEffect::BlockCollisionEffect(size_t numParticles) : sb::ParticleSystem(numParticles), _state(State::Idle)
	{
		setLifetime(0.5f);
		setEmissionRatePerSecond(0);
		setEmissionShape(sb::Box(1, 0.01f));
		setEmissionType(sb::ParticleSystem::EmissionType::Random);
		setParticleLifetimeRange(2.f * sb::Vector2f(0.1f, 0.5f));
		setParticleSpeedRange(sb::Vector2f(0.1f, 1));
		setParticleSizeRange(1.f * sb::Vector2f(0.01f, 0.13f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		setParticleColor(sb::Color::createFromRGB(139, 69, 19, 255));
		setTexture(getTexture());
		getTexture().enableMipmap(true);
	}


	void BlockCollisionEffect::update(float ds) {
		ParticleSystem::update(ds);
		if (_state == State::Playing) {
			if (!sb::ParticleSystem::isPlaying())
				_state = State::Idle;
		}
	}

	void BlockCollisionEffect::play(float delaySeconds, Position direction) {
		setDirection(direction);

		if (_state != State::Playing) {
			clearBursts();
			addBurst(delaySeconds, 20);
			reset();
			_state = State::Playing;
		}
	}
}