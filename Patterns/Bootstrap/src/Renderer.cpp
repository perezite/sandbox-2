#include "Renderer.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>

namespace sb
{
	std::size_t Renderer::m_numDrawCalls = 0;
	
	void Renderer::render(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states)
	{
		if (vertices.empty())
			return;

		Shader* shader = selectShader(states);
		setup(shader, vertices, states);
		drawVertices(vertices, primitiveType);
		cleanup(shader, states);
	}

	std::size_t Renderer::getNumDrawCalls()
	{
		return m_numDrawCalls;
	}

	void Renderer::resetStatistics() 
	{
		m_numDrawCalls = 0;
	}

	Shader* Renderer::selectShader(const DrawStates& states)
	{
		if (states.shader)
			return states.shader;

		if (states.texture)
			return &Shader::getDefaultTextured();

		else
			return &Shader::getDefault();
	}

	inline void enablePointSize() {
		#ifdef WIN32
			GL_CHECK(glEnable(GL_PROGRAM_POINT_SIZE));
		#endif
	}

	void Renderer::setup(Shader* shader, const std::vector<Vertex>& vertices, const DrawStates& states)
	{
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GL_CHECK(glEnable(GL_BLEND));
		enablePointSize();
		GL_CHECK(glActiveTexture(GL_TEXTURE0));

		shader->use();
		setupShaderUniforms(shader, states);
		setupShaderAttributes(shader, vertices, states);
	}

	void Renderer::setupShaderUniforms(Shader* shader, const DrawStates & states)
	{
		shader->setMatrix3("transform", states.transform.getMatrix());
		if (states.texture) {
			shader->setInteger("texture", 0);
			shader->setMatrix3("texTransform", states.textureTransform.getMatrix());
			states.texture->bind();
		}
	}

	void Renderer::setupShaderAttributes(Shader * shader, const std::vector<Vertex>& vertices, const DrawStates & states)
	{
		GLvoid* position = (GLvoid*) &(vertices[0].position);
		GLvoid* color = (GLvoid*) &(vertices[0].color);
		GLvoid* texCoords = (GLvoid*) &(vertices[0].texCoords);
		setShaderAttribute(shader->getAttributeLocation("position"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), position);
		setShaderAttribute(shader->getAttributeLocation("color"), 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), color);
		if (states.texture)
			setShaderAttribute(shader->getAttributeLocation("texCoords"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), texCoords);
	}

	void Renderer::setShaderAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* value)
	{
		GL_CHECK(glVertexAttribPointer(index, size, type, normalized, stride, value));
		GL_CHECK(glEnableVertexAttribArray(index));
	}

	void Renderer::drawVertices(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType)
	{	
		GL_CHECK(glDrawArrays((GLenum)primitiveType, 0, vertices.size()));

		m_numDrawCalls++;
	}

	void Renderer::cleanup(Shader* shader, const DrawStates& states)
	{
		if (states.texture)
			GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("texCoords")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("color")));
		GL_CHECK(glDisableVertexAttribArray(shader->getAttributeLocation("position")));

	}
}