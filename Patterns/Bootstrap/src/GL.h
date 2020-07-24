#pragma once
#include "Logger.h"
#ifdef  WIN32
	#include <gl/glew.h>
	#include <SDL2/SDL_opengl.h>
	#include <gl/glu.h>
#else
	#include <GLES2/gl2.h>	
	#include <GLES2/gl2ext.h>
#endif 
#include <string>

// #define DISABLE_GL_CHECKS 

namespace sb {
	void glCheckError(const char* command, const char* file, unsigned int line);
}

#if defined(_DEBUG) && !defined(DISABLE_GL_CHECKS)
	#define GL_CHECK(cmd)	do { cmd; glCheckError( #cmd, __FILE__, __LINE__); } while (false)
#else
	#define GL_CHECK(cmd) cmd
#endif

