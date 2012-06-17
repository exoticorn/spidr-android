#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include "exo/base/types.hpp"
#include "exo/base/functions.hpp"

namespace exo
{
	GameFramework::GameFramework()
	{
		m_pApplication = nullptr;
	}

	void GameFramework::createApplication()
	{
		m_pApplication = newApplication(*this);
	}

	void GameFramework::destroyApplication()
	{
		delete m_pApplication;
		m_pApplication = nullptr;
	}

	void GameFramework::update()
	{
		m_glContext.update();

		if(m_pApplication)
		{
			float timeStep = min(0.1f, m_timer.getElapsedTime());
			m_timer.reset();
			m_pApplication->update(timeStep);
		}
	}

	void GameFramework::render()
	{
		if(m_pApplication)
		{
			m_pApplication->render();
		}
	}
}
