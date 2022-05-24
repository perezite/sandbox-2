namespace easings 
{
	typedef float(*Easing)(float);
	...
}

namespace 
{
	Easing moveEasing = easings::sineOut;
}

namespace blocks 
{
	class Tetromino 
	{
		void input()
		{
			if (Input::isKeyGoingDown(KeyCode::Left))
				_transformTweener.move(-1, 0, moveEasing, 0.2f);
			if (Input::isKeyGoingDown(KeyCode::Right))
				_transformTweener.move(1, 0, moveEasing, 0.2f);
		}
		
	public:
		void update() 
		{
			input();
		}
	}
}

namespace blocks
{
	class TransformTweener
	{
		float _targetAngle;

	public:
		float getTargetAngle();
	}
}

namespace orbis
{
	inline float signum(float value) 
	{
		if (value == 0)
			return 0;
		
		return value > 0 ? 1 : -1;
	}
}

namespace blocks 
{	
	class BoardCollisionLogic 
	{		
		Tetromino& _lastTetromino;

		float getAngle(Tetromino& tetromino) 
		{
			return tetromino.getTransformTweener().getTargetAngle();
		}
		
		void rotate(Tetromino& tetromino, float angle) 
		{
			tetromino.getTransformTweener().rotate(angle);
		}
		
		bool resolveTetrominoCollisionStep()
		{
			...
			
			float deltaAngle = getAngle(getTetromino) - getAngle(_lastTetromino);
			
			...
			
			if (canResolveCollision)
				rotate(tetromino, -90 * toRadians * signum(deltaAngleSteps));
		}
	}
}