#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include <SDL/SDL.h>

namespace exo
{
	static uint screenWidth = 1024;
	static uint screenHeight = 768;

	GameFrameworkLinux::GameFrameworkLinux()
	{
		setStoragePath(".");
		SDL_Init(SDL_INIT_VIDEO);
		m_pScreen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_OPENGL);
		createApplication();
		getApplication()->setScreenSize(screenWidth, screenHeight);

		m_isMouseDown = false;
		m_touch.id = 0;
		m_touch.x = 0;
		m_touch.y = 0;

		SDL_AudioSpec spec;
		spec.freq = 44100;
		spec.format = AUDIO_S16SYS;
		spec.channels = 2;
		spec.samples = 512;
		spec.callback = &GameFrameworkLinux::audioCallback;
		spec.userdata = getApplication();

		if(SDL_OpenAudio(&spec, NULL) < 0)
		{
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			exit(1);
		}

		SDL_PauseAudio(0);
	}

	GameFrameworkLinux::~GameFrameworkLinux()
	{
		SDL_PauseAudio(1);
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
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						if(!getApplication()->onBackPressed())
						{
							quit = true;
						}
						break;
					default:
						break;
					}
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
		getApplication()->onPause();
	}

	void GameFrameworkLinux::audioCallback(void* pUser, unsigned char* pBuffer, int size)
	{
		static_cast<ApplicationBase*>(pUser)->fillAudioBuffer(reinterpret_cast<sint16*>(pBuffer), (uint)size / 4);
	}
}
