#ifndef EXO_APPLICATION_HPP
#define EXO_APPLICATION_HPP

#include "exo/gameframework/applicationbase.hpp"
#include "level.hpp"
#include "player.hpp"
#include "math.hpp"

class Audio;

namespace exo
{
	enum GameState { State_LevelFadeIn, State_Level, State_LevelFadeOut, State_GameOver, State_Title, State_StartGame, State_ToTitle };

	class Application : public ApplicationBase
	{
	public:
		Application(GameFramework& gameFramework);
		virtual ~Application();

		virtual void	update(float timeStep);
		virtual void	render();

	private:
		Audio*			m_pAudio;
		Level			m_level;
		Player			m_player;
		Input			m_input;
		Vector2			m_mousePosition;

		int				m_currentLevel;
		bool			m_quitGame;
		float			m_scale;
		float			m_stateTime;
		float			m_timeLeft;
		float			m_timeIncrement;
		bool			m_pause;
		int				m_score;
		int				m_hiScore;

		GameState		m_gameState;
		GameState		m_nextState;
	};

	ApplicationBase* newApplication(GameFramework& gameFramework);
}

#endif
