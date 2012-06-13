#ifndef EXO_GAMEFRAMEWORK_HPP
#define EXO_GAMEFRAMEWORK_HPP

#include "exo/gles2/glcontext.hpp"

namespace exo
{
	class ApplicationBase;

	class GameFramework
	{
	public:
		struct Touch
		{
			int			x;
			int			y;
			MemorySize	id;
		};

		GameFramework();
		virtual ~GameFramework() {}

		void				createApplication();
		void				destroyApplication();

		GLContext&			getGLContext() { return m_glContext; }
		ApplicationBase*	getApplication() { return m_pApplication; }

		virtual uint		getNumTouches() const = 0;
		virtual const Touch&	getTouch(uint index) const = 0;

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
