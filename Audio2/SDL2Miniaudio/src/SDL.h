#pragma once
#include "Logger.h"
#include <SDL2/SDL.h>

#define SDL_CHECK(condition) do { if (!!!(condition)) SB_ERROR(SDL_GetError()); } while (false)
