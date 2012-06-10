#include "exo/gameframework/gameframework.hpp"
#include <SDL/SDL.h>

namespace exo
{
	GameFrameworkLinux::GameFrameworkLinux()
	{
		SDL_Init(SDL_INIT_VIDEO);
		m_pScreen = SDL_SetVideoMode(1024, 768, 32, SDL_OPENGL);
		createApplication();
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

			update();
			render();
			SDL_GL_SwapBuffers();
		}
	}
}
