#ifndef EXO_GLRESOURCE_HPP
#define EXO_GLRESOURCE_HPP

namespace exo
{
	class GLContext;

	class GLResource
	{
	public:
		GLResource(GLContext& context);
		virtual ~GLResource();
		virtual void createGLResource() = 0;

	protected:
		bool			m_isGLResourceCreated;

	private:
		GLContext&		m_glContext;

		friend class GLContext;
	};
}

#endif
