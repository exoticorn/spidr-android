#ifndef EXO_GAMEFRAMEWORK_HPP
#define EXO_GAMEFRAMEWORK_HPP

#include "exo/gles2/glcontext.hpp"

namespace exo
{
	class ApplicationBase;

	class GameFramework
	{
	public:
		GameFramework();

		void				createApplication();
		void				destroyApplication();

		GLContext&			getGLContext() { return m_glContext; }

	protected:

		void				update();
		void				render();

	private:
		ApplicationBase*	m_pApplication;
		GLContext			m_glContext;
	};

	extern ApplicationBase* newApplication(GameFramework& gameFramework);
}

#if defined(EXO_PLATFORM_LINUX)
#	include "linux/gameframework_linux.hpp"
#elif defined(EXO_PLATFORM_ANDROID)
#	include "android/gameframework_android.hpp"
#else
#	error "unsupported platform"
#endif

#endif
