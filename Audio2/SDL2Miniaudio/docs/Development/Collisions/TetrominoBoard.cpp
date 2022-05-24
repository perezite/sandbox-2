class Board 
{
public:
	bool tetrominoCollides() const
	{
		const vector<Vector2i>& globalPositions = _tetromino->getGlobalPositions();
		const vector<Vector2i>& blockPositions = getBlockPositions();
		
		for (size_t i = 0; i < globalPositions.size(); i++)
			for (size_t j = 0; j < blockPositions.size(); j++)
				...
	}

	void resolveCollisions() 
	{
		while (tetrominoCollides() && !_tetromino->isStuck()) 
			_tetromino->resolveCollisionStep();
	}
	
	void update() 
	{
		_tetromino->update();
		
		resolveCollisions();
				
		if (_tetromino->isStuck())
			gameOver();
				
		if (_tetromino->isDead())
			respawnTetromino();
	}
}

class Tetromino() 
{
protected:
	int toRightAngle(float angle) 
	{
		return int(angle / (90 * toRadians)) * 90;
	}

	bool hasRotated() const 
	{ 
		return toRightAngle(getRotation()) != toRightAngle(_lastTransformable.getRotation());
	}
	
	bool hasMoved() const 
	{ 
		return toVector2i(getPosition()) != toVector2i(_lastTransformable.getPosition());
	}

	bool hasMovedDown() const 
	{
		toVector2i(getPosition().y) == toVector2i(_lastTransformable.getPosition().y) + 1;
	}

	void resolveRotation() 
	{
		setRotation(_lastTransformable.getRotation());
	}
	
	void resolveMove() 
	{		
		if (hasMovedDown())
			_isDead = true;
	
		setPosition(_lastTransformable.getPosition());			
	}
	
public:
	void resolveCollisionStep() 
	{
		if (hasRotated())
			resolveRotation();
		else if (hasMoved())
			resolveMove();
		else
			_isStuck = true;
	}
	
	const vector<Vector2i>& getGlobalPositions() 
	{
		_globalPositions.resize(_positions.size());
		
		for (size_t i = 0; i < _positions.size(); i++) 
		{
			Vector2f temp = getTransform() * toVector2f(_globalPositions[i]);	
			_globalPositions[i] = toVector2i(temp);
		}
		
		return _globalPositions;
	}
}