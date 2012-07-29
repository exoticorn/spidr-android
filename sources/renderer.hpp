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

		void			beginRendering(float screenWidth, float screenHeight);

		void			translate(float x, float y);
		void			scale(float x, float y);

		void			push();
		void			pop();

		void			drawLines(const float* pVertices, uint numVertices);

		struct Vertex
		{
			Vector2 pos;
			Vector2 normal;
			float	offset;
		};

		struct Line
		{
			Vertex s1, a1, a2, b1, b2, s2;
		};

		void			drawLines(const Line* pLines, uint numLines);

		static void		prepareLines(Line* pBuffer, const float* pVertices, uint numLines);

	private:
		enum {
			MAX_STACK_DEPTH = 16,
			BUFFER_SIZE = 64
		};

		struct StackEntry
		{
			Vector2 scale;
			Vector2 translate;
		};

		StackEntry		m_stack[MAX_STACK_DEPTH];
		uint			m_sp;
		LineShader		m_shader;
		Vector2			m_pixelScale;
		bool			m_isTransformDirty;

		Line			m_buffer[BUFFER_SIZE];
	};
}

#endif
