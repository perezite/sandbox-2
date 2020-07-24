#pragma once
#include "SDL.h"

namespace sb
{
	class Stopwatch
	{
	public:
		Stopwatch();

		void reset();

		float getElapsedMs();

		float getElapsedSeconds();

	private:
		unsigned long m_startTicks;
	};
}