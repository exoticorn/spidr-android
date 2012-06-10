#ifndef EXO_RENDERER_HPP
#define EXO_RENDERER_HPP

#include "line_shader.hpp"
#include "math.hpp"

namespace exo
{
	class Renderer
	{
	public:
		Renderer(GLContext& context);

		void			beginRendering(uint screenWidth, uint screenHeight);

		void			translate(float x, float y);
		void			scale(float x, float y);

		void			push();
		void			pop();

		void			drawLines(const float* pVertices, uint numVertices);

	private:
		enum { MAX_STACK_DEPTH = 16 };

		struct StackEntry
		{
			Vector2 scale;
			Vector2 translate;
		};

		StackEntry		m_stack[MAX_STACK_DEPTH];
		uint			m_sp;
		LineShader		m_shader;
		bool			m_isTransformDirty;
	};
}

#endif
