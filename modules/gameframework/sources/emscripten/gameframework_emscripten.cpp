#include "exo/gameframework/gameframework.hpp"
#include "exo/gameframework/applicationbase.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

namespace exo
{
	static uint screenWidth = 1024;
	static uint screenHeight = 768;

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
	}

	GameFrameworkEmscripten::~GameFrameworkEmscripten()
	{
		SDL_PauseAudio(1);
		destroyApplication();
		SDL_Quit();
	}

	void GameFrameworkEmscripten::run()
	{
		int mouseX, mouseY;
		uint8 mouseButtonState = SDL_GetMouseState(&mouseX, &mouseY);
		m_touch.x = mouseX;
		m_touch.y = mouseY;
		m_isMouseDown = mouseButtonState != 0;

		update();
		render();
		SDL_GL_SwapBuffers();
	}

	void GameFrameworkEmscripten::audioCallback(void* pUser, unsigned char* pBuffer, int size)
	{
		static_cast<ApplicationBase*>(pUser)->fillAudioBuffer(reinterpret_cast<sint16*>(pBuffer), (uint)size / 4);
	}
}
