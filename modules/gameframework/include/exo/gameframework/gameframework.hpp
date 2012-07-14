#ifndef EXO_GAMEFRAMEWORK_HPP
#define EXO_GAMEFRAMEWORK_HPP

#include "exo/gles2/glcontext.hpp"
#include "exo/base/timer.hpp"

namespace exo
{
	class ApplicationBase;

	class GameFramework
	{
	public:
		struct Touch
		{
			float		x;
			float		y;
			MemorySize	id;
		};

		GameFramework();
		virtual ~GameFramework();

		void				createApplication();
		void				destroyApplication();

		GLContext&			getGLContext() { return m_glContext; }
		ApplicationBase*	getApplication() { return m_pApplication; }

		virtual uint		getNumTouches() const = 0;
		virtual const Touch&	getTouch(uint index) const = 0;

		const char*			getStoragePath() const { return m_pStoragePath; }

	protected:

		void				update();
		void				render();

		void				setStoragePath(const char* pPath);

	private:
		ApplicationBase*	m_pApplication;
		GLContext			m_glContext;
		Timer				m_timer;
		float				m_lastTimestep;
		float				m_timeOffset;
		char*				m_pStoragePath;
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
