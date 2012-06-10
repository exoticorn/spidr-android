#include "exo/gles2/shaderbase.hpp"
#include "exo/base/debug.hpp"
#include <string.h>

namespace exo
{
	GLuint compileShader(GLenum shaderType, const char* pSource)
	{
		GLuint shader = glCreateShader(shaderType);
		GLint length = strlen(pSource);
		glShaderSource(shader, 1, &pSource, &length);
		glCompileShader(shader);
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if(status != GL_TRUE)
		{
			char buffer[1024];
			glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
			printAndHalt("Shader compile error:\n%s\n", buffer);
		}
		return shader;
	}

	ShaderBase::ShaderBase(GLContext& context, const SourceData& sourceData)
		: GLResource(context)
		, m_sourceData(sourceData)
	{
		m_shaderProgram = 0;
		m_attributes = nullptr;
		m_uniforms = nullptr;
	}

	ShaderBase::~ShaderBase()
	{
		glDeleteProgram(m_shaderProgram);
		delete [] m_attributes;
		delete [] m_uniforms;
	}

	void ShaderBase::createGLResource()
	{
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, m_sourceData.pVertexSource);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, m_sourceData.pFragmentSource);

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);

		GLint status;
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
		if(status != GL_TRUE)
		{
			char buffer[1024];
			glGetProgramInfoLog(m_shaderProgram, sizeof(buffer), nullptr, buffer);
			printAndHalt("Shader link error:\n%s\n", buffer);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		checkGLError();
		delete [] m_attributes;
		m_attributes = new GLint[m_sourceData.numAttributes];
		for(uint i = 0; i < m_sourceData.numAttributes; ++i)
		{
			m_attributes[i] = glGetAttribLocation(m_shaderProgram, m_sourceData.ppAttributeNames[i]);
		}

		delete [] m_uniforms;
		m_uniforms = new GLint[m_sourceData.numUniforms];
		for(uint i = 0; i < m_sourceData.numUniforms; ++i)
		{
			m_uniforms[i] = glGetUniformLocation(m_shaderProgram, m_sourceData.ppUniformNames[i]);
		}

		checkGLError();
	}

	void ShaderBase::use()
	{
		glUseProgram(m_shaderProgram);
		for(uint i = 0; i < m_sourceData.numAttributes; ++i)
		{
			glEnableVertexAttribArray(m_attributes[i]);
		}
	}
}
