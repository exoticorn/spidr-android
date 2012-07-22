#include "application.hpp"

#include "exo/gameframework/gameframework.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "audio.hpp"
#include "objects.hpp"
#include "math.hpp"
#include "sfx.hpp"
#include "exo/base/functions.hpp"
#include "exo/base/file.hpp"
#include "exo/base/serializer.hpp"

int score = 0;

namespace exo
{
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

		m_scale = 1.0f;
		m_stateTime = 0;
		m_timeLeft = 60;
		m_timeIncrement = 20;
		m_pause = false;
		m_hiScore = 0;
		m_isInGame = false;

		m_gameState = State_ToTitle;
		m_nextState = State_Title;

		load();
	}

	Application::~Application()
	{
	}

	void Application::onPause()
	{
		save();
	}

	void Application::setFadeZoom(float time)
	{
		time = time < 0 ? 0 : (time > 1 ? 1 : time);
		m_scale = pow(2, (1-cosf((time) * 1.5f)) * 8);
	}

	void Application::update(float timeStep)
	{
		m_input.buttonTriggered = false;
		m_input.playDead = false;

		if(m_gameFramework.getNumTouches() > 0)
		{
			m_input.buttonTriggered = !m_input.button;
			m_input.button = true;
			const GameFramework::Touch& touch = m_gameFramework.getTouch(0);
			m_input.stick = Vector2(touch.x, touch.y) - m_screenSize * 0.5f;
		}
		else
		{
			m_input.buttonTriggered = false;
			m_input.button = false;
		}

		switch(m_gameState)
		{
		case State_LevelFadeIn:
			m_player.update(0, m_input);
			setFadeZoom(1 - m_stateTime);
			if(m_stateTime >= 1)
			{
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
				score += (int)m_timeLeft * 10;
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
				if(m_timeLeft < 0)
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
			setFadeZoom(m_stateTime);
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
			score = 0;
			m_isInGame = true;
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
			m_isInGame = false;
			setFadeZoom(1);
			m_level.initialize(&level99);
			m_player.initialize(&m_level);
			FxSynth::playSfx(sfx_level_fade_in);
			break;
		}

		if(score > m_hiScore)
		{
			m_hiScore = score;
		}

		m_stateTime += timeStep;
	}

	void Application::render()
	{
		m_renderer.beginRendering(m_screenSize.x, m_screenSize.y);

		m_renderer.push();
		float uiScale = minf(1.0f, max(m_screenSize.x / 640.0f, m_screenSize.y / 480.0f));
		m_renderer.scale(uiScale, uiScale);
		float xRes = m_screenSize.x / uiScale;
		float yRes = m_screenSize.y / uiScale;
		print(m_renderer, Vector2(10, 10), "hi %d", m_hiScore);
		print(m_renderer, Vector2(xRes - 20 * 11 - 10, 10), "score %d", score);
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
		m_renderer.pop();

		m_renderer.translate(m_screenSize.x / 2, m_screenSize.y / 2);
		float screenScale = min(m_screenSize.x / 640.0f, m_screenSize.y / 480.0f);
		m_renderer.scale(m_scale * screenScale, m_scale * screenScale);
		m_renderer.translate(-m_player.getPosition().x * 100, -m_player.getPosition().y * 100);
		m_renderer.scale(100, 100);

		m_level.render(m_renderer);

		m_player.render(m_renderer, m_gameState == State_Level);
	}

	void Application::setScreenSize(uint width, uint height)
	{
		m_screenSize.set(width, height);
	}

	void Application::fillAudioBuffer(sint16* pBuffer, uint numSamples)
	{
		m_pAudio->fillBuffer(pBuffer, numSamples);
	}

	bool Application::onBackPressed()
	{
		if(m_isInGame)
		{
			m_gameState = State_ToTitle;
			m_nextState = State_Title;
			m_stateTime = 0;
			return true;
		}
		return false;
	}

	static const uint saveVersion = 2;
	static const uint minSaveVersion = 1;

	void Application::load()
	{
		char buffer[128];
		snprintf(buffer, sizeof(buffer), "%s/game.save", m_gameFramework.getStoragePath());
		File file(buffer);
		if(file.isOpened())
		{
			uint size = file.getSize();
			uint8* pBuffer = new uint8[size];
			file.read(pBuffer, size);
			Serializer serializer(pBuffer, size);
			if(serializer.getDataVersion() <= saveVersion && serializer.getDataVersion() >= minSaveVersion &&
					serializer.isValid())
			{
				serialize(serializer);
			}
			delete pBuffer;
		}
	}

	void Application::save()
	{
		char buffer[128];
		snprintf(buffer, sizeof(buffer), "%s/game.save", m_gameFramework.getStoragePath());

		Serializer serializer(saveVersion);
		serialize(serializer);

		File file(buffer, File::Access_Write);
		uint size;
		const uint8* pData = serializer.getData(&size);
		file.write(pData, size);
		delete [] pData;
	}

	void Application::serialize(Serializer& serializer)
	{
		serializer.serialize(&m_hiScore);
		serializer.serialize(&score);
		if(serializer.getDataVersion() < saveVersion)
		{
			return;
		}

		serializer.serialize(&m_isInGame);
		if(!m_isInGame)
		{
			return;
		}

		serializer.serialize(&m_gameState);
		serializer.serialize(&m_nextState);
		serializer.serialize(&m_currentLevel);
		serializer.serialize(&m_stateTime);
		serializer.serialize(&m_timeLeft);
		serializer.serialize(&m_timeIncrement);

		if(serializer.isReading())
		{
			m_level.initialize(pLevels[m_currentLevel]);
			m_player.initialize(&m_level);
		}
		m_level.serialize(serializer);
		m_player.serialize(serializer);
	}

	ApplicationBase* newApplication(GameFramework& gameFramework)
	{
		return new Application(gameFramework);
	}
}
