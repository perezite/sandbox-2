#pragma once
#include "BubbleEffect.h"
#include "ParticleSystem.h"
#include "Boundable.h"
#include "Rect.h"
#include "Box.h"

namespace blocks {
	class BubbleEffect : public sb::ParticleSystem {
		size_t _counter;

	protected:
		static sb::Texture& getTexture();
		void updateEmitter(const sb::FloatRect& bounds, float size);

	public:
		BubbleEffect(size_t maxParticles, const sb::Color& color);
		void setParticleSize(float size);
		void update(Boundable& boundable, Transformable& transformable, float ds);
	};
}