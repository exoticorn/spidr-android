#include "renderer.hpp"
#include "exo/base/debug.hpp"
#include "exo/base/functions.hpp"

namespace exo
{
	Renderer::Renderer(GLContext& context)
		: m_shader(context)
	{
		m_sp = 0;
		m_stack[0].scale.set(1, 1);
		m_stack[0].translate.clear();
		m_pixelScale.clear();
		m_isTransformDirty = true;
	}

	void Renderer::beginRendering(uint screenWidth, uint screenHeight)
	{
		m_sp = 0;
		m_stack[0].scale.set(2.0f / screenWidth, -2.0f / screenHeight);
		m_stack[0].translate.set(-1, 1);
		m_isTransformDirty = true;

		m_pixelScale.set(2.0f / screenWidth, -2.0f / screenHeight);

		glClearColor(0, 0.15f, 0, 0);

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
			m_shader.setPixelScaleUniform(m_pixelScale.x, m_pixelScale.y);
			m_isTransformDirty = false;
		}

		const Vector2* pLines = reinterpret_cast<const Vector2*>(pVertices);
		while(numVertices > 1)
		{
			uint numLines = min<uint>(numVertices / 2, BUFFER_SIZE);

			for(uint i = 0; i < numLines; ++i)
			{
				Vector2 a = *pLines++;
				Vector2 b = *pLines++;
				Vector2 normal = Vector2(a.y - b.y, b.x - a.x).normalize();

				Line& line = m_buffer[i];
				line.a1.pos = a;
				line.a1.normal = normal;
				line.a1.offset = 1.5f;
				line.a2.pos = a;
				line.a2.normal = normal;
				line.a2.offset = -1.5f;
				line.b1.pos = b;
				line.b1.normal = normal;
				line.b1.offset = 1.5f;
				line.b2.pos = b;
				line.b2.normal = normal;
				line.b2.offset = -1.5f;
				line.s1 = line.a1;
				line.s2 = line.b2;
			}

			m_shader.setPositionAttribute(GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_buffer[0].s1.pos);
			m_shader.setNormalOffsetAttribute(GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_buffer[0].s1.normal);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, numLines * 6);

			numVertices -= numLines * 2;
		}
	}
}
