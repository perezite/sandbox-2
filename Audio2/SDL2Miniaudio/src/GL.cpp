#include "GL.h"

namespace sb 
{
	void glCheckError(const char* command, const char* file, unsigned int line)
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
			return;

		std::string code;
		std::string description;

		if (error == GL_INVALID_ENUM) {
			code = "GL_INVALID_ENUM";
			description = "The enumeration parameter is not a legal enumeration for that function";
		}
		else if (error == GL_INVALID_VALUE) {
			code = "GL_INVALID_VALUE";
			description = "The value parameter is not a legal value for that function";
		}
		else if (error == GL_INVALID_OPERATION) {
			code = "GL_INVALID_OPERATION";
			description = "The set of state for a command is not legal for the parameters given to that command";
		}
		else if (error == GL_OUT_OF_MEMORY) {
			code = "GL_OUT_OF_MEMORY";
			description = " the memory cannot be allocated.";
		}
		else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
			code = "GL_INVALID_FRAMEBUFFER_OPERATION";
			description = "Attempting to read from or write/render to a framebuffer that is not complete.";
		}
	#ifndef __ANDROID__
		else if (error == GL_STACK_OVERFLOW) {
			code = "GL_STACK_OVERFLOW";
			description = "A stack pushing operation cannot be done because it would overflow the limit of that stack's size";
		}
		else if (error == GL_STACK_UNDERFLOW) {
			code = "GL_STACK_UNDERFLOW";
			description = "A stack popping operation cannot be done because the stack is already at its lowest point.";
		}
	#endif	
		else {
			code = "Unknown";
			description = "Unknown GL error";
		}

		SB_ERROR("GL Error " << code << " :" << description);
	}
}