class Board 
{
	bool hasTetrominoCollision()
	{
		const vector<Vector2i>& blockPositions = getBlockPositions();
		const vector<Vector2i>& tetrominoPositions = _tetromino.getGlobalPositions();
		
		for (size_t i = 0; i < blockPositions.size(); i++)
			for (size_t j= 0; j < tetrominoPositions.size(); j++)
				if (blockPositions[i] == tetrominoPositions[j])
					return true;
				
		return false;
	}
		
	bool tetrominoCollidedDownwards()
	{
		Vector2f deltaPos = _tetromino.getPosition() - 
							_lastTetrominoTransformable.getPosition();
		bool hadDownwardsCollision = int(deltaPos.y) < 0;
		
		return hadDownwardsCollision;			
	}
	
	void resolveTetrominoRotation() 
	{
		_tetromino.setRotation(_lastTetrominoTransformable.getRotation());
	}

	void resolveTetrominoPosition() 
	{
		_tetromino.setPosition(_lastTetrominoTransformable.getPosition());
		_isTetrominoDead = tetrominoCollidedDownwards();
	}

	bool resolveTetrominoStep() 
	{
		float deltaRotation = (_tetromino.getRotation() - _lastTetrominoTransformable.getRotation()) * toDegrees;
		int deltaRotationSteps = round(deltaRotation / 90);
		Vector2i deltaPosition = toVector2f(tetromino.getPosition() - _lastTetrominoTransformable.getPosition();
		
		bool canResolveCollision = deltaRotation != 0 || deltaPosition != Vector2i(0);
		if (!canResolveCollision)
			return false;
		
		if (deltaRotationSteps != 0)
			_tetromino.rotate(deltaRotationSteps > 0 ? 90 * toRadians : - 90 * toRadians);
		else if (deltaPosition.y < 0) 
		{
			_tetromino.translate(0, 1);
			_isTetrominoDead = true;		// a vertical downwards collision means that the tetromino died			 
		}
		else if (deltaPosition.y > 0)
			_tetromino.translate(0, -1);
		else if (deltaPosition.x != 0)
			_tetromino.translate();
		
		return true;
	}
	
	void resolveTetrominoCollisions() 
	{	
		while (hasTetrominoCollision()) 
		{
			bool couldResolveCollision = resolveTetrominoCollisionStep();
			if (!couldResolveCollision)
			{
				_isTetrominoStuck = true;
				break;
			}			
		}	
	}
	
	void updateTetrominoCollisions()
	{
		resolveTetrominoCollisions();		

		if (_isTetrominoDead)
			respawnTetromino();

		if (_isTetrominoStuck)
			gameOver();
	}
	
	void update() 
	{
		_lastTetrominoTransformable = (Transformable)_tetromino;
		
		_tetromino.update();	
		updateTetrominoCollisions();
	}
}

class Tetromino 
{
	const vector<Vector2i>& getGlobalPositions() 
	{
		_globalPositions.resize(_positions.size());
		
		for (size_t i = 0; i < _positions.size(); i++) 
		{
			Vector2f temp = getTransform() * 
							toVector2f(_globalPositions[i]);	
			_globalPositions[i] = toVector2i(temp);
		}
		
		return _globalPositions;
	}
}