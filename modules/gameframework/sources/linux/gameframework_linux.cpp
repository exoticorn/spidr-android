#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include <SDL/SDL.h>

namespace exo
{
	GameFrameworkLinux::GameFrameworkLinux()
	{
		SDL_Init(SDL_INIT_VIDEO);
		m_pScreen = SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL);
		createApplication();
		getApplication()->setScreenSize(1024, 768);

		m_isMouseDown = false;
		m_touch.id = 0;
		m_touch.x = 0;
		m_touch.y = 0;
	}

	GameFrameworkLinux::~GameFrameworkLinux()
	{
		destroyApplication();
		SDL_Quit();
	}

	void GameFrameworkLinux::run()
	{
		bool quit = false;
		while(!quit)
		{
			SDL_Event event;
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				}
			}

			int mouseX, mouseY;
			uint8 mouseButtonState = SDL_GetMouseState(&mouseX, &mouseY);
			m_touch.x = mouseX;
			m_touch.y = mouseY;
			m_isMouseDown = mouseButtonState != 0;

			update();
			render();
			SDL_GL_SwapBuffers();
		}
	}
}
