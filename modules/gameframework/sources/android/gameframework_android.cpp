#include "exo/gameframework/gameframework.hpp"
#include "exo/gles2/glcontext.hpp"
#include "exo/gameframework/applicationbase.hpp"

namespace exo
{
	GameFrameworkAndroid::GameFrameworkAndroid()
	{
		createApplication();
		m_touch.id = 0;
	}

	GameFrameworkAndroid::~GameFrameworkAndroid()
	{
		destroyApplication();
	}

	uint GameFrameworkAndroid::getNumTouches() const
	{
		return 0;
	}

	const GameFramework::Touch& GameFrameworkAndroid::getTouch(uint index) const
	{
		EXO_USE_PARAMETER(index);
		return m_touch;
	}

	void GameFrameworkAndroid::surfaceCreated()
	{
		getGLContext().invalidateAll();
	}

	void GameFrameworkAndroid::update()
	{
		GameFramework::update();
		render();
	}

	void GameFrameworkAndroid::setScreenSize(uint width, uint height)
	{
		getApplication()->setScreenSize(width, height);
	}
}
