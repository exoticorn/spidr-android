#include "exo/gles2/glresource.hpp"
#include "exo/gles2/glcontext.hpp"

namespace exo
{
	GLResource::GLResource(GLContext& context)
		: m_glContext(context)
	{
		m_isGLResourceCreated = false;
		context.registerResource(this);
	}

	GLResource::~GLResource()
	{
		m_glContext.unregisterResource(this);
	}
}
