#include "exo/gameframework/gameframework.hpp"
#include "exo/gles2/glcontext.hpp"
#include "exo/gameframework/applicationbase.hpp"

namespace exo
{
	GameFrameworkAndroid::GameFrameworkAndroid()
	{
		createApplication();
	}

	GameFrameworkAndroid::~GameFrameworkAndroid()
	{
		destroyApplication();
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
