#include "application.hpp"

#include "exo/gameframework/gameframework.hpp"

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

	Application::Application(GameFramework& gameFramework)
		: ApplicationBase(gameFramework)
		, m_renderer(gameFramework.getGLContext())
		, m_startButton("start game")
		, m_continueButton("continue")
		, m_pauseButton("#")
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

		m_pContinueSave = nullptr;
		m_continueSaveSize = 0;

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
		m_pause = true;
	}

	void Application::setFadeZoom(float time)
	{
		time = time < 0 ? 0 : (time > 1 ? 1 : time);
		m_scale = pow(2, (1-cosf((time) * 1.5f)) * 8);
	}

	void Application::update(float timeStep)
	{
		MutexLock lock(m_updateMutex);
		m_input.buttonTriggered = false;
		m_input.playDead = false;

		float uiScale = getUIScale();
		float xRes = m_screenSize.x / uiScale;
		float yRes = m_screenSize.y / uiScale;

		if(m_pContinueSave)
		{
			m_startButton.update(timeStep, xRes / 2 - 40 - m_startButton.getWidth(), (yRes - m_startButton.getHeight()) / 2 + 50);
			m_continueButton.update(timeStep, xRes / 2 + 40, (yRes - m_continueButton.getHeight()) / 2 + 50);
		}
		else
		{
			m_startButton.update(timeStep, (xRes - m_startButton.getWidth()) / 2, (yRes - m_startButton.getHeight()) / 2 + 50);
		}
		m_pauseButton.update(timeStep, xRes - 60, 60);
		m_pauseButton.setText(m_pause ? ">" : "#");

		if(m_gameFramework.getNumTouches() > 0)
		{
			m_input.buttonTriggered = !m_input.button;
			m_input.button = true;
			const GameFramework::Touch& touch = m_gameFramework.getTouch(0);
			m_input.stick = Vector2(touch.x, touch.y) - m_screenSize * 0.5f;
			m_input.pos = Vector2(touch.x, touch.y) * (1 / uiScale);
		}
		else
		{
			m_input.buttonTriggered = false;
			m_input.button = false;
		}

		if(m_isInGame)
		{
			if(m_pauseButton.handleInput(m_input))
			{
				m_pause = !m_pause;
			}
		}

		switch(m_gameState)
		{
		case State_LevelFadeIn:
			m_player.update(0, m_input);
			setFadeZoom(1 - m_stateTime);
			if(m_stateTime >= 1)
			{
				if(m_nextState == State_Title)
				{
					m_startButton.fadeIn();
					if(m_pContinueSave)
					{
						m_continueButton.fadeIn();
					}
				}
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
			m_startButton.fadeOut();
			m_continueButton.fadeOut();
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
			if(m_startButton.handleInput(m_input))
			{
				FxSynth::playSfx(sfx_collect, 1, true);
				m_gameState = State_LevelFadeOut;
				m_nextState = State_StartGame;
				m_stateTime = 0;
			}
			else if(m_pContinueSave && m_continueButton.handleInput(m_input))
			{
				Serializer serializer(m_pContinueSave, m_continueSaveSize);
				serialize(serializer);
				m_startButton.fadeOut();
				m_continueButton.fadeOut();
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
			delete [] m_pContinueSave;
			m_pContinueSave = nullptr;
			m_continueSaveSize = 0;
			m_isInGame = true;
			m_pause = false;
			break;
		case State_GameOver:
			m_input.playDead = true;
			m_player.update(timeStep, m_input);
			if(m_stateTime > 4)
			{
				delete [] m_pContinueSave;
				m_pContinueSave = nullptr;
				m_continueSaveSize = 0;
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

		if(m_isInGame)
		{
			m_pauseButton.fadeIn();
		}
		else
		{
			m_pauseButton.fadeOut();
		}

		if(score > m_hiScore)
		{
			m_hiScore = score;
		}

		m_stateTime += timeStep;
	}

	float Application::getUIScale() const
	{
		return minf(1.0f, max(m_screenSize.x / 640.0f, m_screenSize.y / 480.0f));
	}

	void Application::render()
	{
		m_renderer.beginRendering(m_screenSize.x, m_screenSize.y);
		
		m_renderer.push();
		float uiScale = getUIScale();
		m_renderer.scale(uiScale, uiScale);
		float xRes = m_screenSize.x / uiScale;
		float yRes = m_screenSize.y / uiScale;
		print(m_renderer, Vector2(10, 10), 0, "hi %d", m_hiScore);
		print(m_renderer, Vector2(xRes - 20 * 11 - 10, 10), 0, "score %d", score);
		if(m_gameState != State_Title && m_nextState != State_Title && m_nextState != State_StartGame)
		{
			print(m_renderer, Vector2(10, yRes - 32), 0, "$%d", m_level.numOrbsLeft());
			int intTimeLeft = ceil(m_timeLeft);
			print(m_renderer, Vector2(xRes / 2 - 40, yRes - 32), 0, "%d:%02d", intTimeLeft / 60, intTimeLeft % 60);
		}

		if(m_gameState == State_GameOver && fmodf(m_stateTime, 1) < 0.5f)
		{
			print(m_renderer, Vector2(xRes/2 - 90, yRes/2 + 50), 0, "game over");
		}

		m_startButton.render(m_renderer);
		m_continueButton.render(m_renderer);
		m_pauseButton.render(m_renderer);

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

	static const uint saveVersion = 4;
	static const uint minSaveVersion = 1;
	static const uint minHashVersion = 4;

	bool Application::onBackPressed()
	{
		if(m_isInGame)
		{
			Serializer serializer(saveVersion);
			serialize(serializer);
			m_pContinueSave = serializer.getData(&m_continueSaveSize);
			m_gameState = State_ToTitle;
			m_nextState = State_Title;
			m_stateTime = 0;
			return true;
		}
		return false;
	}

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
					serializer.isValid(serializer.getDataVersion() >= minHashVersion))
			{
				serialize(serializer);
			}
			delete pBuffer;
		}
	}

	void Application::save()
	{
		MutexLock lock(m_updateMutex);
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
		if(m_isInGame)
		{
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
			m_pause = true;
		}
		else
		{
			serializer.serialize(&m_continueSaveSize);
			if(m_continueSaveSize)
			{
				if(serializer.isReading())
				{
					delete [] m_pContinueSave;
					m_pContinueSave = new uint8[m_continueSaveSize];
				}
				serializer.serialize(const_cast<uint8*>(m_pContinueSave), m_continueSaveSize);
			}
		}
	}

	ApplicationBase* newApplication(GameFramework& gameFramework)
	{
		return new Application(gameFramework);
	}
}
