#include "Block.h"
#include "DrawTarget.h"

namespace blocks {
	sb::Texture& Block::getSheet() {
		static sb::Texture texture("Textures/Blocks.png");
		return texture;
	}

	inline const sb::IntRect Block::getTextureArea(std::size_t x, std::size_t y) {
		static sb::Vector2i blockSize(128, 128);
		return sb::IntRect(x * blockSize.x, y * blockSize.y, blockSize.x, blockSize.y);
	}

	void Block::setTexture(char type) {
		if (type == 'i')
			_sprite.setTexture(getSheet(), getTextureArea(1, 2));
		else if (type == 'j')
			_sprite.setTexture(getSheet(), getTextureArea(2, 2));
		else if (type == 'l')
			_sprite.setTexture(getSheet(), getTextureArea(1, 1));
		else if (type == 'o')
			_sprite.setTexture(getSheet(), getTextureArea(0, 2));
		else if (type == 's')
			_sprite.setTexture(getSheet(), getTextureArea(0, 1));
		else if (type == 't')
			_sprite.setTexture(getSheet(), getTextureArea(0, 0));
		else if (type == 'z')
			_sprite.setTexture(getSheet(), getTextureArea(2, 1));
		else if (type == 'm')
			_sprite.setTexture(getSheet(), getTextureArea(1, 2));
		else
			SB_ERROR("Invalid Tetromino type " << type);
	}

	sb::Vector2f Block::transformDirection(const sb::Transform& transform, sb::Vector2f& dir) {
		const float* m = transform.getMatrix();
		return sb::Vector2f(
			m[0] * dir.x + m[3] * dir.y,
			m[1] * dir.x + m[4] * dir.y);
	}

	void Block::updateLighting(const sb::Transform& transform) {
		if (_light) {
			sb::Vector2f up(0, 1);
			sb::Vector2f transformedUp = transformDirection(transform, up);
			float angle = sb::angle(-_light->getDirection(), transformedUp) + 45 * sb::ToRadian;
			angle = angle < 0 ? angle + 2 * sb::Pi : angle;
			int steps = int(angle / (90 * sb::ToRadian));
			_sprite.setRotation(-steps * 90 * sb::ToRadian);
		}
	}

	void Block::implode(float ds) {
		if (!_effects.isPlaying()) {
			_explosion.explode();
			_implosionStartTransform = getTransform();
			_state = State::Exploding;
		}
	}

	void Block::explode(float ds) {
		if (!_explosion.isActive() || !_isExplosionEnabled)
			_state = State::Garbage;
	}

	Block::Block(char type) : _light(NULL), _state(State::Alive), _explosion(128, type), _collisionEffect(128),
		_isExplosionEnabled(true)
	{
		_explosion.setScale(1.25f);
		_collisionEffect.setPosition(0, 0.5f);
		setType(type);
	}

	std::map<char, sb::Color>& Block::getColors()
	{
		static const int alpha = 200;
		static std::map<char, sb::Color> colors = {
			{ 'i', sb::Color::createFromRGB(0, 240, 240, alpha) },
			{ 'j', sb::Color::createFromRGB(0, 0, 240, alpha) },
			{ 'l', sb::Color::createFromRGB(240, 160, 0, alpha) },
			{ 'o', sb::Color::createFromRGB(240, 240, 0, alpha) },
			{ 's', sb::Color::createFromRGB(0, 240, 0, alpha) },
			{ 't', sb::Color::createFromRGB(160, 0, 240, alpha) },
			{ 'z', sb::Color::createFromRGB(240, 0, 0, alpha) },
			{ 'm', sb::Color::createFromRGB(0, 240, 240, alpha) }
		};

		return colors;
	}

	void Block::setType(char type) {
		type = tolower(type);
		setTexture(type);
	}

	void Block::die(float duration) {
		getEffects().implode(*this, duration);
		_state = State::Imploding;
	}

	void Block::update(float ds) {
		_effects.update(ds);
		_explosion.update(ds);
		_collisionEffect.update(ds);

		if (_state == State::Imploding)
			implode(ds);
		if (_state == State::Exploding)
			explode(ds);
	}

	void Block::drawExplosion(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= _implosionStartTransform;
		target.draw(_explosion, _implosionStartTransform);
	}

	void Block::drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		target.draw(_collisionEffect, states);
	}

	void Block::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
		updateLighting(states.transform);
		target.draw(_sprite, states);
	}
}