#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include "exo/base/types.hpp"

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
			m_pApplication->update(1.0f / 60);
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
