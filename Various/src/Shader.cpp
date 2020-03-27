#include "Shader.h"
#include "Logger.h"
#include "Asset.h"

namespace sb
{
	Shader::Shader(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) 
	{
		loadFromAsset(vertexShaderCode, fragmentShaderCode);
	}

	Shader& Shader::getDefault()
	{
		static Shader defaultShader;
		static bool isLoaded = false;
		if (!isLoaded) {
			defaultShader.loadFromMemory(getDefaultVertexShaderCode(), getDefaultFragmentShaderCode());
			isLoaded = true;
		}
		return defaultShader;
	}

	Shader& Shader::getDefaultTextured()
	{
		static Shader defaultTexturedShader;
		static bool isLoaded = false;
		if (!isLoaded) {
			defaultTexturedShader.loadFromMemory(getDefaultTexturedVertexShaderCode(), getDefaultTexturedFragmentShaderCode());
			isLoaded = true;
		}
		return defaultTexturedShader;
	}

	GLint Shader::getAttributeLocation(std::string attribute)
	{
		if (m_attributeLocations.find(attribute) == m_attributeLocations.end()) {
			GLint location;
			GL_CHECK(location = glGetAttribLocation(m_handle, attribute.c_str()));
			SB_ERROR_IF(location < 0, "Requested shader attribute " << attribute << " is not available");
			m_attributeLocations[attribute] = location;
			return location;
		}

		return m_attributeLocations[attribute];
	}

	void Shader::setMatrix3(std::string uniformName, const float* matrix3)
	{
		GLuint uniformLocation = getUniformLocation(uniformName);
		GL_CHECK(glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, matrix3));
	}

	void Shader::setInteger(std::string uniformName, int value)
	{
		GLuint uniformLocation = getUniformLocation(uniformName);
		GL_CHECK(glUniform1i(uniformLocation, value));
	}


	GLuint Shader::getUniformLocation(std::string uniform)
	{
		if (m_uniformLocations.find(uniform) == m_uniformLocations.end()) {
			GLuint location;
			GL_CHECK(location = glGetUniformLocation(m_handle, uniform.c_str()));
			m_uniformLocations[uniform] = location;
			return location;
		}

		return m_uniformLocations[uniform];
	}

	void Shader::loadFromMemory(const std::string& vertexShaderCode, const std::string& fragmentShaderCode) 
	{
		GL_CHECK(m_handle = glCreateProgram());
		SB_ERROR_IF(m_handle == 0, "Creating shader program failed");

		GLuint vertexShader = compile(vertexShaderCode, GL_VERTEX_SHADER);
		GLuint fragmentShader = compile(fragmentShaderCode, GL_FRAGMENT_SHADER);

		GL_CHECK(glAttachShader(m_handle, vertexShader));
		GL_CHECK(glAttachShader(m_handle, fragmentShader));

		link();

		GL_CHECK(glDeleteShader(vertexShader));
		GL_CHECK(glDeleteShader(fragmentShader));
	}

	void Shader::loadFromAsset(const std::string & vertexShaderAssetPath, const std::string & fragmentShaderAssetPath)
	{
		loadFromMemory(Asset::readAllText(vertexShaderAssetPath), Asset::readAllText(fragmentShaderAssetPath));
	}

	void Shader::use() const
	{
		GL_CHECK(glUseProgram(m_handle));
	}

	void Shader::destroy()
	{
		GL_CHECK(glDeleteProgram(m_handle));
	}

	GLuint Shader::compile(std::string shaderCode, GLenum type)
	{
		GLint compiled;
		GLuint shader;
		GL_CHECK(shader = glCreateShader(type));

		if (shader != 0) {
			const char* shaderCodeStr = shaderCode.c_str();
			GL_CHECK(glShaderSource(shader, 1, &shaderCodeStr, NULL));
			GL_CHECK(glCompileShader(shader));

			GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
			if (!compiled) {
				GLint infoLen = 0;
				GL_CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen));

				if (infoLen > 1) {
					char* infoLog = new char[infoLen];
					GL_CHECK(glGetShaderInfoLog(shader, infoLen, NULL, infoLog));
					SB_ERROR("error compiling shader: " << infoLog);
					delete[] infoLog;
				}
				GL_CHECK(glDeleteShader(shader));
				shader = 0;
			}
		}

		return shader;
	}

	void Shader::link()
	{
		GL_CHECK(glLinkProgram(m_handle));
		GLint linked;
		GL_CHECK(glGetProgramiv(m_handle, GL_LINK_STATUS, &linked));
		if (!linked) {
			GLint infoLen = 0;
			GL_CHECK(glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLen));

			if (infoLen > 1) {
				char* infoLog = new char[infoLen];
				GL_CHECK(glGetProgramInfoLog(m_handle, infoLen, NULL, infoLog));
				SB_ERROR("Error linking shader program: " << std::endl << infoLog);
				delete[] infoLog;
			}

			GL_CHECK(glDeleteProgram(m_handle));
		}
	}

	std::string Shader::getDefaultVertexShaderCode()
	{	
		return
			"attribute vec2 position;																\n"
			"attribute vec4 color;																	\n"
			"uniform mat3 transform;																\n"
			"varying vec4 v_color;																	\n"
			"vec3 transformedPosition;																\n"
			"void main()																			\n"
			"{																						\n"
			"	transformedPosition = transform * vec3(position.x, position.y, 1);					\n"
			"	gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1 );			\n"
			"	gl_PointSize = 2.0;																	\n"
			"	v_color = color;																	\n"
			"}																						\n";
	}

	std::string Shader::getDefaultFragmentShaderCode()
	{
		return
			"#version 100									\n"
			"precision mediump float;						\n"
			"varying vec4 v_color;		 					\n"
			"void main()									\n"
			"{												\n"
			"	gl_FragColor = v_color;						\n"
			"}												\n";
	}

	std::string Shader::getDefaultTexturedVertexShaderCode()
	{
		return
			"attribute vec2 position;																\n"
			"attribute vec4 color;																	\n"
			"attribute vec2 texCoords;																\n"
			"uniform mat3 transform;																\n"
			"varying vec4 v_color;																	\n"
			"varying vec2 v_texCoords;																\n"
			"vec3 transformedPosition;																\n"
			"void main()																			\n"
			"{																						\n"
			"	transformedPosition = transform * vec3(position.x, position.y, 1);					\n"
			"	gl_Position = vec4(transformedPosition.x, transformedPosition.y, 0, 1);				\n"
			"	gl_PointSize = 2.0;																	\n"
			"	v_color = color;																	\n"
			"	v_texCoords = texCoords;															\n"
			"}																						\n";
	}

	std::string Shader::getDefaultTexturedFragmentShaderCode()
	{
		return
			"#version 100																		\n"
			"precision mediump float;															\n"
			"varying vec4 v_color;																\n"
			"varying vec2 v_texCoords;															\n"
			"uniform sampler2D texture;															\n"
			"uniform mat3 texTransform;															\n"
			"vec3 transformedTexCoords;															\n"
			"void main()																		\n"
			"{																					\n"
			"	transformedTexCoords = texTransform * vec3(v_texCoords.x, v_texCoords.y, 1);	\n"
			"	gl_FragColor = texture2D(texture, transformedTexCoords.xy) * v_color;			\n"
			"}																					\n";
	}
}