#pragma once

#include "Stopwatch.h"

namespace blocks 
{
	class Timer
	{
		sb::Stopwatch _stopwatch;

		float _intervalSeconds;

		float _timerSeconds;

	public:
		Timer(float intervalSeconds)
			: _intervalSeconds(intervalSeconds), _timerSeconds(0)
		{ }

		bool hasTicks(); 
	};
}