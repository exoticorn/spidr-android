#ifndef EXO_GL_H
#define EXO_GL_H

#if defined(EXO_PLATFORM_LINUX)
#	define GL_GLEXT_PROTOTYPES
#	include <GL/gl.h>
#	include <GL/glext.h>
#elif defined(EXO_PLATFORM_ANDROID)
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#elif defined(EXO_PLATFORM_EMSCRIPTEN)
#	include <GLES2/gl2.h>
#	include <GLES2/gl2ext.h>
#else
#	error "unsupported platform"
#endif

namespace exo
{
	void checkGLError();
}

#endif
