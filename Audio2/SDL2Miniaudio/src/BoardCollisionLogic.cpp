#include "BoardCollisionLogic.h"
#include "Board.h"
#include "Math.h"
#include "VectorHelper.h"

using namespace std;
using namespace sb;

namespace blocks
{
	Tetromino& BoardCollisionLogic::getTetromino()
	{
		return _parent.getTetromino();
	}

	bool BoardCollisionLogic::tetrominoIsOutsideBounds()
	{
		vector<Vector2i> positions; 
		getTetromino().getTransformedSquarePositions(positions);
		IntRect boardRect(0, 0, _parent.getSize().x, _parent.getSize().y);

		for (size_t i = 0; i < positions.size(); i++)
			if (!boardRect.contains(positions[i]))
				return true;

		return false;
	}

	bool BoardCollisionLogic::tetrominoCollidesWithBlock()
	{
		vector<Vector2i> blockPositions; 
		vector<Vector2i> tetrominoPositions; 
		_parent.getBlockPositions(blockPositions);
		getTetromino().getTransformedSquarePositions(tetrominoPositions);

		for (size_t i = 0; i < blockPositions.size(); i++)
			for (size_t j = 0; j < tetrominoPositions.size(); j++)
				if (blockPositions[i] == tetrominoPositions[j])
					return true;

		return false;
	}

	bool BoardCollisionLogic::resolveTetrominoCollisionStep()
	{
		float deltaAngle = getTetromino().getRotation() - _lastTetromino.getRotation();
		int deltaAngleSteps = (int)round(deltaAngle * ToDegrees / 90);
		Vector2i deltaPosition = toVector2i(getTetromino().getPosition() - _lastTetromino.getPosition());
		bool canResolveCollision = deltaAngleSteps != 0 || deltaPosition != Vector2i(0);

		if (canResolveCollision) {
			if (deltaAngleSteps != 0)
				getTetromino().rotate(deltaAngleSteps > 0 ? -90 * ToRadians : 90 * ToRadians);

			else if (deltaPosition.y < 0) {
				getTetromino().translate(0, 1);
				_isTetrominoDead = true;        // downwards collisions kill the tetromino
			}

			else if (deltaPosition.y > 0)
				getTetromino().translate(0, -1);

			else if (deltaPosition.x != 0)
				getTetromino().translate(deltaPosition.x > 0 ? Vector2f(-1, 0) : Vector2f(1, 0));
		}

		return canResolveCollision;
	}

	void BoardCollisionLogic::resolveTetrominoCollisions()
	{
		while (hasTetrominoCollision()) {
			bool collisionResolved = resolveTetrominoCollisionStep();
			if (!collisionResolved) {
				_isTetrominoStuck = true;
				break;
			}
		}
	}

	void BoardCollisionLogic::updateTetrominoCollisions()
	{
		if (_mustResetTetrominoData) {
			resetTetrominoData();
			_mustResetTetrominoData = false;
		}

		resolveTetrominoCollisions();
		_lastTetromino = (Transformable)_parent.getTetromino();

		if (_mustResetTetrominoData) {
			resetTetrominoData();
			_mustResetTetrominoData = false;
		}

		resolveTetrominoCollisions();

		_lastTetromino = (Transformable)_parent.getTetromino();
	}

	bool BoardCollisionLogic::hasTetrominoCollision()
	{
		bool hasCollision = tetrominoCollidesWithBlock();
		hasCollision |= tetrominoIsOutsideBounds();

		return hasCollision;
	}

	void BoardCollisionLogic::resetTetrominoData()
	{
		_lastTetromino = (Transformable)_parent.getTetromino();
		_isTetrominoDead = false;
		_isTetrominoStuck = false;
	}

	void BoardCollisionLogic::update()
	{
		updateTetrominoCollisions();
	}
}