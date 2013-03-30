#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

namespace exo
{
	static uint screenWidth = 640;
	static uint screenHeight = 400;

	GameFrameworkEmscripten::GameFrameworkEmscripten()
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
		spec.callback = &GameFrameworkEmscripten::audioCallback;
		spec.userdata = getApplication();

		if(SDL_OpenAudio(&spec, NULL) < 0)
		{
			fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
			exit(1);
		}

		SDL_PauseAudio(0);

		m_fpsTimer.reset();
		m_fpsCount = 0;
	}

	GameFrameworkEmscripten::~GameFrameworkEmscripten()
	{
		SDL_PauseAudio(1);
		destroyApplication();
		SDL_Quit();
	}

	void GameFrameworkEmscripten::run()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_MOUSEMOTION:
				m_touch.x = event.motion.x;
				m_touch.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_isMouseDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				m_isMouseDown = false;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					getApplication()->onBackPressed();
					break;
				default:
					break;
				}
				break;
			case SDL_VIDEORESIZE:
				getApplication()->setScreenSize((uint)event.resize.w, (uint)event.resize.h);
				break;
			}
		}

		update();
		render();
		SDL_GL_SwapBuffers();

		++m_fpsCount;
		float fpsTime = m_fpsTimer.getElapsedTime();
		if(fpsTime >= 1)
		{
//			printf("fps: %f\n", m_fpsCount / fpsTime);
			m_fpsCount = 0;
			m_fpsTimer.reset();
		}
	}

	void GameFrameworkEmscripten::audioCallback(void* pUser, unsigned char* pBuffer, int size)
	{
		static_cast<ApplicationBase*>(pUser)->fillAudioBuffer(reinterpret_cast<sint16*>(pBuffer), (uint)size / 4);
	}
}
