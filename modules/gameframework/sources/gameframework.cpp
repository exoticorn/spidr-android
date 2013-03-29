#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include "exo/base/types.hpp"
#include "exo/base/functions.hpp"
#include <malloc.h>
#include <string.h>

namespace exo
{
	GameFramework::GameFramework()
	{
		m_pApplication = nullptr;
		m_lastTimestep = 1.0f / 60;
		m_timeOffset = 0.0f;
		m_pStoragePath = nullptr;
	}

	GameFramework::~GameFramework()
	{
		free(m_pStoragePath);
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
			float target = m_timeOffset + m_timer.getElapsedTime();
			float timeStep = m_lastTimestep + (target - m_lastTimestep) * 0.2f;
			m_timeOffset = clamp(-0.1f, 0.1f, target - timeStep);
			timeStep = min(0.1f, timeStep);
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

	void GameFramework::setStoragePath(const char* pPath)
	{
		m_pStoragePath = strdup(pPath);
	}
}
