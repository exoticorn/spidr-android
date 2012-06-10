#include "application.hpp"

#include "exo/gameframework/gameframework.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "audio.hpp"
#include "objects.hpp"
#include "math.hpp"
#include "sfx.hpp"

int score = 0;

namespace exo
{
	int xRes = 1024;
	int yRes = 768;

	const float mouseCursor[] = {
		-6, -6,		-2, -2,
		6, -6,		2, -2,
		-6, 6,		-2, 2,
		6, 6,		2, 2
	};

	static const struct { char c; const float* pObject; } fontMapping[] = {
		{ '0', obj_0 },
		{ '1', obj_1 },
		{ '2', obj_2 },
		{ '3', obj_3 },
		{ '4', obj_4 },
		{ '5', obj_5 },
		{ '6', obj_6 },
		{ '7', obj_7 },
		{ '8', obj_8 },
		{ '9', obj_9 },
		{ 's', obj_5 },
		{ 'c', obj_c },
		{ 'o', obj_0 },
		{ 'r', obj_r },
		{ 'e', obj_e },
		{ 'h', obj_h },
		{ 'i', obj_i },
		{ '/', obj_slash },
		{ '$', obj_orb },
		{ ':', obj_colon },
		{ 'g', obj_6 },
		{ 'a', obj_a },
		{ 'm', obj_m },
		{ 'v', obj_v },
		{ 'p', obj_p },
		{ 't', obj_t },
	};

	void print(Renderer& renderer, const Vector2& position, const char* pFormat, ...)
	{
		va_list ap;
		va_start(ap, pFormat);
		char buffer[100];
		vsnprintf(buffer, sizeof(buffer), pFormat, ap);
		va_end(ap);

		renderer.push();
		renderer.translate(position.x, position.y);
		renderer.scale(20, 20);
		const char* pText = buffer;
		while(*pText)
		{
			for(int i = 0; i < (int)(sizeof(fontMapping) / sizeof(fontMapping[0])); i++)
			{
				if(fontMapping[i].c == *pText)
				{
					renderer.drawLines(fontMapping[i].pObject + 1, (uint)*fontMapping[i].pObject);
					break;
				}
			}
			renderer.translate(1, 0);
			pText++;
		}
		renderer.pop();
	}

	Application::Application(GameFramework& gameFramework)
		: ApplicationBase(gameFramework)
		, m_renderer(gameFramework.getGLContext())
	{
		m_pAudio = new Audio;

		m_currentLevel = 0;

		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2);

		glEnableClientState(GL_VERTEX_ARRAY);

		glClearColor(0, 1, 0, 0);
		m_quitGame = false;

		m_scale = 1.0f;
		m_stateTime = 0;
		m_timeLeft = 60;
		m_timeIncrement = 20;
		m_pause = false;
		m_hiScore = 0;

		m_gameState = State_ToTitle;
		m_nextState = State_Title;
	}

	Application::~Application()
	{
	}

	void Application::update(float timeStep)
	{
		m_mousePosition = Vector2(xRes / 2, yRes / 2);

		m_input.buttonTriggered = false;
		m_input.playDead = false;

		/*
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					if(gameState == State_LevelFadeIn || gameState == State_Level || gameState == State_LevelFadeOut)
					{
						quitGame = true;
					}
					else
					{
						quit = true;
					}
					break;
				case SDLK_LSHIFT:
				case SDLK_SPACE:
					input.button = input.buttonTriggered = true;
					break;
				case SDLK_p:
					if(gameState == State_Level)
					{
						pause = !pause;
					}
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				input.button = false;
				break;
			case SDL_JOYBUTTONDOWN:
				input.button = input.buttonTriggered = true;
				break;
			case SDL_JOYBUTTONUP:
				input.button = false;
				break;
			case SDL_MOUSEMOTION:
				mousePosition.x = event.motion.x;
				mousePosition.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				input.button = input.buttonTriggered = true;
				break;
			case SDL_MOUSEBUTTONUP:
				input.button = false;
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		if(pPad)
		{
			input.stick = Vector2(SDL_JoystickGetAxis(pPad, 0) / 32768.0, SDL_JoystickGetAxis(pPad, 1) / 32768.0);
		}
		else
		{
			input.stick = (mousePosition - Vector2(xRes/2, yRes/2)) * (2.0 / yRes);
		}
		*/

		switch(m_gameState)
		{
		case State_LevelFadeIn:
			m_player.update(0, m_input);
			if(m_stateTime < 1)
			{
				m_scale = pow(2, (1-cosf((1 - m_stateTime) * 1.5f)) * 8);
			}
			else
			{
				m_scale = 1;
				m_gameState = m_nextState;
			}
			break;
		case State_Level:
			if(!m_pause)
			{
				m_player.update(timeStep, m_input);
			}
			if(m_level.numOrbsLeft() == 0)
			{
				m_score += (int)m_timeLeft * 10;
				m_gameState = State_LevelFadeOut;
				m_nextState = State_LevelFadeIn;
				m_stateTime = 0;
				m_pause = false;
			}
			else
			{
				if(!m_pause)
				{
					m_timeLeft -= timeStep;
				}
				if(m_timeLeft < 0 || m_quitGame)
				{
					m_timeLeft = 0;
					m_gameState = State_GameOver;
					m_stateTime = 0;
					FxSynth::playSfx(sfx_gameover, 1, true);
					m_pause = false;
				}
			}
			break;
		case State_LevelFadeOut:
			m_input.playDead = true;
			m_player.update(timeStep, m_input);
			m_scale = pow(2, ((1-cosf(m_stateTime)) * 1.5f) * 8);
			if(m_stateTime > 1)
			{
				m_currentLevel++;
				if(m_currentLevel >= numLevels)
				{
					m_currentLevel = 0;
				}
				m_level.initialize(pLevels[m_currentLevel]);
				m_player.initialize(&m_level);
				m_gameState = m_nextState;
				if(m_nextState == State_LevelFadeIn)
				{
					FxSynth::playSfx(sfx_level_fade_in);
					m_nextState = State_Level;
					m_timeLeft += m_timeIncrement;
					m_timeIncrement *= 0.85f;
				}
				m_stateTime = 0;
			}
			break;
		case State_Title:
			m_input.playDead = true;
			m_player.update(timeStep, m_input);
			if(m_input.buttonTriggered)
			{
				FxSynth::playSfx(sfx_collect, 1, true);
				m_gameState = State_LevelFadeOut;
				m_nextState = State_StartGame;
				m_stateTime = 0;
			}
			break;
		case State_StartGame:
			FxSynth::playSfx(sfx_level_fade_in);
			m_gameState = State_LevelFadeIn;
			m_nextState = State_Level;
			m_timeLeft = 60;
			m_timeIncrement = 20;
			m_currentLevel = 0;
			m_level.initialize(pLevels[m_currentLevel]);
			m_player.initialize(&m_level);
			m_stateTime = 0;
			m_score = 0;
			m_quitGame = false;
			break;
		case State_GameOver:
			m_input.playDead = true;
			m_player.update(timeStep, m_input);
			if(m_stateTime > 4)
			{
				m_gameState = State_LevelFadeOut;
				m_nextState = State_ToTitle;
				m_stateTime = 0;
			}
			break;
		case State_ToTitle:
			m_gameState = State_LevelFadeIn;
			m_nextState = State_Title;
			m_stateTime = 0;
			m_level.initialize(&level99);
			m_player.initialize(&m_level);
			FxSynth::playSfx(sfx_level_fade_in);
			break;
		}

		if(m_score > m_hiScore)
		{
			m_hiScore = m_score;
		}

		m_stateTime += timeStep;
		score = m_score;
	}

	void Application::render()
	{
		m_renderer.beginRendering(xRes, yRes);
		glClearColor(0, 0.1f, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		{
			m_renderer.push();
			m_renderer.translate(m_mousePosition.x, m_mousePosition.y);
			m_renderer.drawLines(mouseCursor, sizeof(mouseCursor) / (sizeof(float) * 2));
			m_renderer.pop();
		}

		print(m_renderer, Vector2(10, 10), "hi %d", m_hiScore);
		print(m_renderer, Vector2(xRes - 20 * 11 - 10, 10), "score %d", m_score);
		if(m_gameState != State_Title && m_nextState != State_Title && m_nextState != State_StartGame)
		{
			print(m_renderer, Vector2(10, yRes - 32), "$%d", m_level.numOrbsLeft());
			int intTimeLeft = ceil(m_timeLeft);
			print(m_renderer, Vector2(xRes / 2 - 40, yRes - 32), "%d:%02d", intTimeLeft / 60, intTimeLeft % 60);
		}

		if(m_gameState == State_Title && fmodf(m_stateTime, 1) < 0.5f)
		{
			print(m_renderer, Vector2(xRes/2 - 110, yRes/2 + 50), "press start");
		}
		else if(m_gameState == State_GameOver && fmodf(m_stateTime, 1) < 0.5f)
		{
			print(m_renderer, Vector2(xRes/2 - 90, yRes/2 + 50), "game over");
		}

		m_renderer.scale(m_scale, m_scale);
		m_renderer.translate(xRes/2/m_scale - m_player.getPosition().x * 100, yRes/2/m_scale - m_player.getPosition().y * 100);
		m_renderer.scale(100, 100);

		m_level.render(m_renderer);

		m_player.render(m_renderer, m_gameState == State_Level);
	}

	ApplicationBase* newApplication(GameFramework& gameFramework)
	{
		return new Application(gameFramework);
	}
}
