#ifndef EXO_GAMEFRAMEWORK_HPP
#define EXO_GAMEFRAMEWORK_HPP

namespace exo
{
	class ApplicationBase;

	class GameFramework
	{
	public:
		GameFramework();

		void				createApplication();
		void				destroyApplication();

	protected:

		void				update();
		void				render();

	private:
		ApplicationBase*	m_pApplication;
	};

	extern ApplicationBase* newApplication(GameFramework& gameFramework);
}

#if defined(EXO_BUILD_LINUX)
#	include "linux/gameframework_linux.hpp"
#elif defined(EXO_BUILD_ANDROID)
#	include "android/gameframework_android.hpp"
#endif

#endif
