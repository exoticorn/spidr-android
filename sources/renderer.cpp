#include "renderer.hpp"
#include "exo/base/debug.hpp"
#include "exo/base/functions.hpp"
#include "objects.hpp"
#include <stdarg.h>
#include <stdio.h>

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

	void Renderer::beginRendering(float screenWidth, float screenHeight)
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

		glViewport(0, 0, screenWidth, screenHeight);
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
		while(numVertices > 1)
		{
			uint numLines = min<uint>(numVertices / 2, BUFFER_SIZE);

			prepareLines(m_buffer, pVertices, numLines);
			pVertices += numLines * 4;

			drawLines(m_buffer, numLines);

			numVertices -= numLines * 2;
		}
	}

	void Renderer::drawLines(const Line* pLines, uint numLines)
	{
		m_shader.use();
		if(m_isTransformDirty)
		{
			m_shader.setScaleUniform(m_stack[m_sp].scale.x, m_stack[m_sp].scale.y);
			m_shader.setTranslationUniform(m_stack[m_sp].translate.x, m_stack[m_sp].translate.y);
			m_shader.setPixelScaleUniform(m_pixelScale.x, m_pixelScale.y);
			m_isTransformDirty = false;
		}

		m_shader.setPositionAttribute(GL_FLOAT, GL_FALSE, sizeof(Vertex), &pLines->s1.pos);
		m_shader.setNormalOffsetAttribute(GL_FLOAT, GL_FALSE, sizeof(Vertex), &pLines->s1.normal);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, numLines * 6);
	}

	void Renderer::prepareLines(Line* pBuffer, const float* pVertices, uint numLines)
	{
		const Vector2* pLines = reinterpret_cast<const Vector2*>(pVertices);

		for(uint i = 0; i < numLines; ++i)
		{
			Vector2 a = *pLines++;
			Vector2 b = *pLines++;
			Vector2 normal = Vector2(a.y - b.y, b.x - a.x).normalize();

			Line& line = pBuffer[i];
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
	}

	static const struct { char c; const float* pObject; } fontMapping[] = {
		{ '0', obj_0 },
		{ '1', obj_1 },
		{ '2', obj_2 },
		{ '3', obj_3 },
		{ '4', obj_4 },
		{ '5', obj_5 },
		{ '6', obj_6 },
		{ '7', obj_7 },
		{ '8', obj_8 },
		{ '9', obj_9 },
		{ 's', obj_5 },
		{ 'c', obj_c },
		{ 'o', obj_0 },
		{ 'r', obj_r },
		{ 'e', obj_e },
		{ 'h', obj_h },
		{ 'i', obj_i },
		{ '/', obj_slash },
		{ '$', obj_orb },
		{ ':', obj_colon },
		{ 'g', obj_6 },
		{ 'a', obj_a },
		{ 'm', obj_m },
		{ 'v', obj_v },
		{ 'p', obj_p },
		{ 't', obj_t },
		{ 'n', obj_n },
		{ 'u', obj_u },
		{ '#', obj_pause },
		{ '>', obj_play }
	};

	void print(Renderer& renderer, const Vector2& position, float fade, const char* pFormat, ...)
	{
		va_list ap;
		va_start(ap, pFormat);
		char buffer[100];
		vsnprintf(buffer, sizeof(buffer), pFormat, ap);
		va_end(ap);

		renderer.push();
		renderer.translate(position.x, position.y);
		renderer.scale(20, 20);
		const char* pText = buffer;
		while(*pText)
		{
			if(fade < 1)
			{
				for(int i = 0; i < (int)(sizeof(fontMapping) / sizeof(fontMapping[0])); i++)
				{
					if(fontMapping[i].c == *pText)
					{
						if(fade <= 0)
						{
							renderer.drawLines(fontMapping[i].pObject + 1, (uint)*fontMapping[i].pObject);
						}
						else
						{
							Vector2 buffer[64];
							uint numLines = exo::min<uint>(countof(buffer) / 2, (uint)*fontMapping[i].pObject / 2);
							const Vector2* pSource = (const Vector2*)(fontMapping[i].pObject + 1);
							for(uint i = 0; i < numLines; ++i)
							{
								Vector2 a = *pSource++;
								Vector2 b = *pSource++;
								buffer[i*2+0] = a;
								buffer[i*2+1] = a + (b - a) * (1 - fade);
							}
							renderer.drawLines(&buffer[0].x, numLines * 2);
						}
						break;
					}
				}
			}
			fade -= 1;
			renderer.translate(1, 0);
			pText++;
		}
		renderer.pop();
	}
}
