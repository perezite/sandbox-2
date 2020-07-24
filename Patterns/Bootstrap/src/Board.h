#pragma once
#include "Drawable.h"
#include "Transformable.h"
#include "DrawBatch.h"
#include "Grid.h"
#include "StripedQuad.h"
#include "Border.h"
#include "Block.h"
#include "Light.h"
#include "Tetromino.h"
#include "SoundEffect.h"

namespace blocks {
	class Board : public sb::Drawable, public sb::Transformable {
		sb::DrawBatch _batch;
		sb::Vector2i _boardSize;

		Grid _grid;
		StripedQuad _stripes;
		Border _border;
		bool _isGridEnabled;
		std::vector<Block> _blocks;
		Light _light;
		Tetromino _tetromino;
		bool _hasTetromino;
		float _stepIntervalInSeconds;
		float _secondsSinceLastStep;
		bool _isFull;
		size_t _linesCleared;
		bool _isAutodropEnabled;
		std::vector<Tetromino> _dyingTetrominoes;
		SoundEffect _explosionSound;
		SoundEffect _popSound;
		SoundEffect _spinSound;

	protected:
		void createTetromino(Tetromino& tetromino, const sb::Vector2i& boardPosition);
		void createRandomTetromino();
		sb::Vector2f boardToWorldPosition(const sb::Vector2i& boardPos);
		inline int floori(float x) { return int(floor(x)); }
		sb::Vector2i worldToBoardPosition(const sb::Vector2f& worldPos);
		sb::Vector2f getCellSize();
		void moveBy(Tetromino& tetromino, const sb::Vector2i& translation);
		void driftBy(Tetromino& tetromino, const sb::Vector2i& translation, float effectSeconds = .2f);
		bool isOccupied(const sb::Vector2i& position);
		bool isOutsideBoard(const sb::Vector2i& position);
		bool isInvalid(Tetromino& tetromino);
		inline bool isValid(Tetromino& tetromino) { return !isInvalid(tetromino); }
		void freeze(Tetromino& tetromino);
		bool isLineFull(size_t y);
		void implodeLine(size_t y);
		void dropBlocksAboveLine(int y);
		void clearLine(size_t y);
		void clearLines();
		void drop(Tetromino& tetromino);
		void step(Tetromino& tetromino, float ds);
		bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos, const sb::Vector2i& dir);
		bool isReachable(Tetromino& tetromino, const sb::Vector2f& targetPos);
		static bool isGarbage(const Block& block);
		static bool isTetrominoGarbage(Tetromino& tetromino);
		void dispose();
		void bounceBy(Block& block, const sb::Vector2i& translation, float duration = 0.5f);
		int getLowestLineWithBlocks();
		bool isInFinalPosition(Tetromino& tetromino);

	public:
		Board(const sb::Vector2i& boardSize);
		inline bool isDead() const { return _isFull; }
		inline bool hasTetromino() const { return _hasTetromino; }
		inline Tetromino& getTetromino() { return _tetromino; }
		inline std::vector<Block>& getBlocks() { return _blocks; }
		inline const sb::Vector2i& getBoardSize() const { return _boardSize; }
		inline size_t getLinesCleared() const { return _linesCleared; }
		sb::Vector2f getSize() const;
		sb::FloatRect getBounds();
		inline void showGrid(bool enabled) { _isGridEnabled = enabled; }
		inline void enableAutodrop(bool enable) { _isAutodropEnabled = enable; }
		inline void setStepInterval(float dropIntervalInSeconds) { _stepIntervalInSeconds = dropIntervalInSeconds; }
		void die();
		void createBlock(char type, const sb::Vector2i& position);
		void createTetromino(char type, const sb::Vector2i& boardPosition);
		void createTetromino(char type);
		void driftTetrominoTo(sb::Vector2f pos, float effectSeconds = 0.2f);
		void driftTetrominoBy(const sb::Vector2i& translation);
		inline void driftTetrominoBy(int x, int y) { driftTetrominoBy(sb::Vector2i(x, y)); }
		void spinTetromino();
		void popTetromino();
		void dropTetromino();
		void hardDropTetromino();
		void dropBlocks();
		void implodeBlocks(float duration = 0.8f);
		void clearLowestLineWithBlocks();
		Tetromino computeProjection();
		void updateEntities(float ds);
		void updateSounds(float ds);
		void update(float ds);
		virtual void draw(sb::DrawTarget& target, sb::DrawStates states = sb::DrawStates::getDefault());
	};
}
