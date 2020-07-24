#pragma once
#include "ParticleSystem.h"

namespace blocks
{
	class BlockCollisionEffect : public sb::ParticleSystem {
	public:
		enum struct Position { Left, Top, Right, Bottom };

	private:
		enum class State { Idle, Playing };
		State _state;

	protected:
		static sb::Texture& getTexture();
		void setDirection(Position direction);

	public:
		BlockCollisionEffect(size_t numParticles);
		inline bool isPlaying() const { return _state == State::Playing; }
		virtual void update(float ds);
		void play(float delaySeconds = 0, Position direction = Position::Top);
	};
}