#pragma once

#include "Drawable.h"
#include "Vertex.h"
#include "Shader.h"
#include "PrimitiveType.h"
#include "Transform.h"
#include <vector>

namespace sb
{
	class Renderer
	{
	public:
		void render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states);

		static std::size_t getNumDrawCalls();
		
		static void resetStatistics();

	protected:
		Shader* selectShader(const DrawStates& states);
		
		void setup(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& states);

		void setupShaderUniforms(Shader* shader, const DrawStates& states);

		void setupShaderAttributes(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& states);

		void setShaderAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);

		void drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType);

		void cleanup(Shader* shader, const DrawStates& states);

	private:
		static std::size_t m_numDrawCalls;
	};
}