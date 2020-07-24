#pragma once
#include "Drawable.h"
#include "BlockCollisionEffect.h"
#include "Transformable.h"
#include "BlockExplosion.h"
#include "TransformEffects.h"
#include "Sprite.h"
#include "Light.h"

namespace blocks
{
	class Block : public sb::Drawable, public sb::Transformable {
	public:
		enum struct State {
			Alive, Imploding, Exploding, Garbage
		};

	private:
		sb::Sprite _sprite;
		const Light* _light;
		State _state;
		BlockExplosion _explosion;
		BlockCollisionEffect _collisionEffect;
		bool _isExplosionEnabled;
		sb::Transform _implosionStartTransform;
		TransformEffects _effects;

	protected:
		static sb::Texture& getSheet();
		inline const sb::IntRect getTextureArea(std::size_t x, std::size_t y);
		void setTexture(char type);
		sb::Vector2f transformDirection(const sb::Transform& transform, sb::Vector2f& dir);
		void updateLighting(const sb::Transform& transform);
		void implode(float ds);
		void explode(float ds);

	public:
		Block(char type = 'i');
		static std::map<char, sb::Color>& getColors();
		static inline sb::Color& getColor(char type) { return Block::getColors()[type]; }
		inline const State& getState() const { return _state; }
		inline TransformEffects& getEffects() { return _effects; }
		inline const BlockCollisionEffect& getCollisionEffect() const { return _collisionEffect; }
		inline BlockCollisionEffect& getCollisionEffect() { return _collisionEffect; }
		inline void setColor(const sb::Color& color) { _sprite.setColor(color); }
		void setType(char type);
		inline void setLight(const Light& light) { _light = &light; }
		inline void enableExplosion(bool enable) { _isExplosionEnabled = enable; }
		void die(float duration = 0.8f);
		void update(float ds);
		void drawExplosion(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
		void drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
	};
}