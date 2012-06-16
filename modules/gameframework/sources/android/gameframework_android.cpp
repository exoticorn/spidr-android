#include "exo/gameframework/android/gameframework_android.hpp"
#include "exo/gles2/glcontext.hpp"
#include "exo/gameframework/applicationbase.hpp"

namespace exo
{
	GameFrameworkAndroid::GameFrameworkAndroid()
	{
		createApplication();
		m_touch.id = 0;
		m_hasTouch = false;
	}

	GameFrameworkAndroid::~GameFrameworkAndroid()
	{
		destroyApplication();
	}

	uint GameFrameworkAndroid::getNumTouches() const
	{
		return m_hasTouch ? 1u : 0u;
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

	void GameFrameworkAndroid::handleTouchInput(int id, bool down, float x, float y)
	{
		EXO_USE_PARAMETER(id);
		m_hasTouch = down;
		m_touch.x = x;
		m_touch.y = y;
	}
}
