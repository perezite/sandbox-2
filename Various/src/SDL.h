#pragma once
#include "Logger.h"
#include <SDL2/SDL.h>

namespace sb {
	void glCheckError(const char* command, const char* file, unsigned int line);
}

#define SDL_CHECK(condition) do { if (!!!(condition)) SB_ERROR(SDL_GetError()); } while (false)
