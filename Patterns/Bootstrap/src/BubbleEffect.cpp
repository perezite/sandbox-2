#include "BubbleEffect.h"

namespace blocks {
	sb::Texture& BubbleEffect::getTexture() {
		static sb::Texture texture("Textures/Particle2.png");
		return texture;
	}

	void BubbleEffect::updateEmitter(const sb::FloatRect& bounds, float size) {
		if ((_counter % 15 == 0) && getState() == sb::ParticleSystem::State::Alive) {
			setPosition(bounds.left + 0.5f * bounds.width, bounds.top());
			setScale(size);
			setEmissionShape(sb::Box(bounds.width / size, 0.01f));

			float maxLength = std::max(bounds.width, bounds.height);
			float factor = (bounds.width / maxLength);
			setEmissionRatePerSecond(factor * 5);
		}

		_counter++;
	}

	BubbleEffect::BubbleEffect(size_t maxParticles, const sb::Color& color) : sb::ParticleSystem(maxParticles), _counter(0) {
		setTexture(getTexture());
		setParticleColor(color);
		setEmissionRatePerSecond(0);
		setParticleSizeRange(1.5f * sb::Vector2f(0.1f, 0.13f));
		setParticleScaleOverLifetime(sb::Tweenf().backInOut(1, 1.5f, 0.2f).sineOut(1.5f, 0, 0.8f));
		setEmissionShape(sb::Box(1, 0.01f));
		setEmissionType(sb::ParticleSystem::EmissionType::Directional);
		setEmissionDirection(sb::Vector2f(0, 1));
		setParticleSpeedRange(sb::Vector2f(2.5f));
		setScale(0.3f);
	}

	void BubbleEffect::setParticleSize(float size) {
		setParticleSizeRange(size * sb::Vector2f(0.1f, 0.13f));
	}

	void BubbleEffect::update(Boundable& boundable, Transformable& transformable, float ds) {
		sb::ParticleSystem::update(ds);
		updateEmitter(boundable.getBounds(), transformable.getScale().x);
	}
}

