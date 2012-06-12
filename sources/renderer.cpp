#include "renderer.hpp"
#include "exo/base/debug.hpp"

namespace exo
{
	Renderer::Renderer(GLContext& context)
		: m_shader(context)
	{
		m_sp = 0;
		m_stack[0].scale.set(1, 1);
		m_stack[0].translate.clear();
		m_isTransformDirty = true;
	}

	void Renderer::beginRendering(uint screenWidth, uint screenHeight)
	{
		m_sp = 0;
		m_stack[0].scale.set(2.0f / screenWidth, -2.0f / screenHeight);
		m_stack[0].translate.set(-1, 1);
		m_isTransformDirty = true;

		glClearColor(0, 0.1f, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void Renderer::scale(float x, float y)
	{
		m_stack[m_sp].scale.x *= x;
		m_stack[m_sp].scale.y *= y;
		m_isTransformDirty = true;
	}

	void Renderer::translate(float x, float y)
	{
		m_stack[m_sp].translate.x += m_stack[m_sp].scale.x * x;
		m_stack[m_sp].translate.y += m_stack[m_sp].scale.y * y;
		m_isTransformDirty = true;
	}

	void Renderer::push()
	{
		ASSERTE(m_sp < MAX_STACK_DEPTH);
		m_stack[m_sp + 1] = m_stack[m_sp];
		++m_sp;
	}

	void Renderer::pop()
	{
		ASSERTE(m_sp > 0);
		--m_sp;
		m_isTransformDirty = true;
	}

	void Renderer::drawLines(const float* pVertices, uint numVertices)
	{
		m_shader.use();
		if(m_isTransformDirty)
		{
			m_shader.setScaleUniform(m_stack[m_sp].scale.x, m_stack[m_sp].scale.y);
			m_shader.setTranslationUniform(m_stack[m_sp].translate.x, m_stack[m_sp].translate.y);
			m_isTransformDirty = false;
		}
		m_shader.setPositionAttribute(GL_FLOAT, GL_FALSE, 8, pVertices);
		glDrawArrays(GL_LINES, 0, numVertices);
	}
}
