#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "Boundable.h"
#include "Block.h"
#include "BubbleEffect.h"
#include "SoundEffect.h"
#include "Boundable.h"
#include <vector>

namespace blocks {
	class Tetromino : public sb::Drawable, public sb::Transformable, public Boundable {
	public:
		enum class State { Alive, Dying, Garbage };

	private:
		char _type;
		std::vector<Block> _blocks;
		std::vector<sb::Vector2i> _blockPositions;
		sb::IntRect _blockBounds;
		TransformEffects _effects;
		BubbleEffect _bubbleEffect;
		SoundEffect _collisionSound;
		State _state;

	protected:
		void clear();
		void computeBlockBounds();
		void createBlocks(const std::vector<sb::Vector2i>& positions, char type);
		void createBlocks(char type);
		bool isInInterval(float value, float min, float max);
		std::vector<int> getXBlockCoordinates();
		std::vector<int> getYBlockCoordinates();
		sb::Vector2i getMinimumBlockCoordinates();
		sb::Vector2i getMaximumBlockCoordinates();
		std::vector<size_t> getBlocksInRow(int y);
		std::vector<size_t> getBlocksInColumn(int x);
		float getProperRotation(float degrees);
		std::vector<size_t> getBottomBlocks();
		BlockCollisionEffect::Position getBlockCollisionEffectPosition();
		bool isCollisionEffectPlaying() const;
		void updateDying();

	public:
		Tetromino(char type = 'i');
		inline State getState() const { return _state; }
		inline TransformEffects& getEffects() { return _effects; }
		inline char getType() const { return _type; }
		static const std::vector<char> getTypes();
		const std::vector<sb::Vector2f> getBlockPositions();
		inline const sb::Color& getColor() const { return Block::getColor(_type); }
		void setColor(const sb::Color& color);
		void setType(char type);
		void setLight(Light& light);
		inline const sb::IntRect& getBlockBounds() const { return _blockBounds; }
		sb::FloatRect getBounds();
		void die();
		void playCollisionEffect(float secondsDelay);
		void update(float ds);
		void drawCollisionEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
		void drawBubbleEffect(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
	};
}