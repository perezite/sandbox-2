#include "Tetromino.h"
#include "DrawTarget.h"
#include "Math.h"

namespace blocks {

	void Tetromino::clear() {
		_blocks.clear();
		_blockPositions.clear();
	}

	void Tetromino::computeBlockBounds() {
		sb::Vector2i minimum, maximum;
		computeBounds(_blockPositions, minimum, maximum);
		_blockBounds = sb::IntRect(minimum.x, minimum.y, maximum.x - minimum.x + 1, maximum.y - minimum.y + 1);
	}

	void Tetromino::createBlocks(const std::vector<sb::Vector2i>& positions, char type) {
		computeBlockBounds();
		for (size_t i = 0; i < positions.size(); i++) {
			Block block;
			block.setPosition((float)positions[i].x, (float)positions[i].y);
			block.setType(type);
			_blocks.push_back(block);
		}
	}

	void Tetromino::createBlocks(char type) {
		if (type == 'i')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(2, 0) };
		else if (type == 'j')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(1, -1) };
		else if (type == 'l')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(-1, -1) };
		else if (type == 'o')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, 1), sb::Vector2i(1, 1) };
		else if (type == 's')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1), sb::Vector2i(-1, -1) };
		else if (type == 't')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(1, 0), sb::Vector2i(0, -1) };
		else if (type == 'z')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(-1, 0), sb::Vector2i(0, -1), sb::Vector2i(1, -1) };
		else if (type == 'm')
			_blockPositions = { sb::Vector2i(0, 0), sb::Vector2i(1, 0) };

		createBlocks(_blockPositions, type);
	}

	bool Tetromino::isInInterval(float value, float min, float max) {
		return value >= min && value < max;
	}

	std::vector<int> Tetromino::getXBlockCoordinates() {
		std::vector<int> xCoordinates(_blockPositions.size());
		for (size_t i = 0; i < xCoordinates.size(); i++)
			xCoordinates[i] = _blockPositions[i].x;
		return xCoordinates;
	}

	std::vector<int> Tetromino::getYBlockCoordinates() {
		std::vector<int> yCoordinates(_blockPositions.size());
		for (size_t i = 0; i < yCoordinates.size(); i++)
			yCoordinates[i] = _blockPositions[i].y;
		return yCoordinates;
	}

	sb::Vector2i Tetromino::getMinimumBlockCoordinates() {
		std::vector<int> xCoordinates = getXBlockCoordinates();
		std::vector<int> yCoordinates = getYBlockCoordinates();
		auto minX = *std::min_element(xCoordinates.begin(), xCoordinates.end());
		auto minY = *std::min_element(yCoordinates.begin(), yCoordinates.end());
		return sb::Vector2i(minX, minY);
	}

	sb::Vector2i Tetromino::getMaximumBlockCoordinates() {
		std::vector<int> xCoordinates = getXBlockCoordinates();
		std::vector<int> yCoordinates = getYBlockCoordinates();
		auto maxX = *std::max_element(xCoordinates.begin(), xCoordinates.end());
		auto maxY = *std::max_element(yCoordinates.begin(), yCoordinates.end());
		return sb::Vector2i(maxX, maxY);
	}

	std::vector<size_t> Tetromino::getBlocksInRow(int y) {
		std::vector<size_t> blocksInRow;
		for (size_t i = 0; i < _blockPositions.size(); i++) {
			if (_blockPositions[i].y == y)
				blocksInRow.push_back(i);
		}

		return blocksInRow;
	}

	std::vector<size_t> Tetromino::getBlocksInColumn(int x) {
		std::vector<size_t> blocksInColumn;
		for (size_t i = 0; i < _blockPositions.size(); i++) {
			if (_blockPositions[i].x == x)
				blocksInColumn.push_back(i);
		}
		return blocksInColumn;
	}

	float Tetromino::getProperRotation(float degrees) {
		degrees = fmod(degrees, 360.f);
		degrees = degrees > 0 ? 360 - degrees : -degrees;
		return degrees;
	}

	std::vector<size_t> Tetromino::getBottomBlocks() {
		float rotation = getProperRotation(getRotation() * sb::ToDegrees);
		if (isInInterval(rotation, 45, 135))
			return getBlocksInColumn(getMaximumBlockCoordinates().x);
		else if (isInInterval(rotation, 135, 225))
			return getBlocksInRow(getMaximumBlockCoordinates().y);
		else if (isInInterval(rotation, 225, 315))
			return getBlocksInColumn(getMinimumBlockCoordinates().x);
		else
			return getBlocksInRow(getMinimumBlockCoordinates().y);
	}

	BlockCollisionEffect::Position Tetromino::getBlockCollisionEffectPosition() {
		float rotation = getProperRotation(getRotation() * sb::ToDegrees);
		if (isInInterval(rotation, 45, 135))
			return BlockCollisionEffect::Position::Right;
		else if (isInInterval(rotation, 135, 225))
			return BlockCollisionEffect::Position::Top;
		else if (isInInterval(rotation, 225, 315))
			return BlockCollisionEffect::Position::Left;
		else
			return BlockCollisionEffect::Position::Bottom;
	}

	bool Tetromino::isCollisionEffectPlaying() const {
		for (size_t i = 0; i < _blocks.size(); i++) {
			if (_blocks[i].getCollisionEffect().isPlaying())
				return true;
		}

		return false;
	}

	void Tetromino::updateDying() {
		if (!isCollisionEffectPlaying() && _bubbleEffect.getState() == sb::ParticleSystem::State::Garbage)
			_state = State::Garbage;
	}

	Tetromino::Tetromino(char type) : _bubbleEffect(256, Block::getColors()[type]), _state(State::Alive) {
		setType(type);
		_collisionSound.loadFromAssetPool("Sounds/Collision.wav");
	}

	const std::vector<char> Tetromino::getTypes() {
		return{ 'i', 'j', 'l', 'o', 's', 't', 'z' };
	}

	const std::vector<sb::Vector2f> Tetromino::getBlockPositions() {
		std::vector<sb::Vector2f> blockPositions;
		blockPositions.reserve(_blocks.size());
		for (size_t i = 0; i < _blocks.size(); i++)
			blockPositions.push_back(getTransform() * _blocks[i].getPosition());

		return blockPositions;
	}

	void Tetromino::setColor(const sb::Color& color) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].setColor(color);
	}

	void Tetromino::setType(char type) {
		clear();
		_type = tolower(type);
		createBlocks(_type);
	}

	void Tetromino::setLight(Light& light) {
		for (std::size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].setLight(light);
	}


	sb::FloatRect Tetromino::getBounds() {
		sb::Vector2f blockSize(1.f, 1.f);
		sb::FloatRect bounds(_blockBounds.left - 0.5f, _blockBounds.bottom - 0.5f, (float)_blockBounds.width, (float)_blockBounds.height);

		return  getTransform() * bounds;
	}

	void Tetromino::die() {
		_bubbleEffect.die();
		_state = State::Dying;
	}

	void Tetromino::playCollisionEffect(float secondsDelay) {
		std::vector<size_t> bottomBlocks = getBottomBlocks();
		BlockCollisionEffect::Position effectPosition = getBlockCollisionEffectPosition();

		bool playSound = false;
		for (size_t i = 0; i < bottomBlocks.size(); i++) {
			BlockCollisionEffect& blockEffect = _blocks[bottomBlocks[i]].getCollisionEffect();
			playSound = playSound || !blockEffect.isPlaying();
			blockEffect.play(secondsDelay, effectPosition);
		}

		if (playSound)
			_collisionSound.play(secondsDelay);
	}

	void Tetromino::update(float ds) {
		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].update(ds);

		_effects.update(ds);
		_bubbleEffect.update(*this, *this, ds);
		_collisionSound.update(ds);

		if (_state == State::Dying)
			updateDying();
	}

	void Tetromino::drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();

		for (size_t i = 0; i < _blocks.size(); i++)
			_blocks[i].drawCollisionEffect(target, states);
	}

	void Tetromino::drawBubbleEffect(sb::DrawTarget& target, sb::DrawStates states) {
		target.draw(_bubbleEffect, states);
	}

	void Tetromino::draw(sb::DrawTarget& target, sb::DrawStates states) {
		states.transform *= getTransform();
		_effects.apply(states.transform);
		for (size_t i = 0; i < _blocks.size(); i++)
			target.draw(_blocks[i], states);
	}
}