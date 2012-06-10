#include "exo/gles2/gl.h"
#include "exo/base/debug.hpp"

namespace exo
{
	void checkGLError()
	{
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			BREAK("GL error 0x%x\n", error);
		}
	}
}
