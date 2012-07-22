#ifndef EXO_APPLICATION_HPP
#define EXO_APPLICATION_HPP

#include "exo/gameframework/applicationbase.hpp"
#include "level.hpp"
#include "player.hpp"
#include "math.hpp"
#include "renderer.hpp"

class Audio;

namespace exo
{
	enum GameState { State_LevelFadeIn, State_Level, State_LevelFadeOut, State_GameOver, State_Title, State_StartGame, State_ToTitle };

	class Serializer;

	class Application : public ApplicationBase
	{
	public:
		Application(GameFramework& gameFramework);
		virtual ~Application();

		virtual void	update(float timeStep);
		virtual void	render();
		virtual void	setScreenSize(uint width, uint height);
		virtual void	fillAudioBuffer(sint16* pBuffer, uint numSamples);
		virtual void	onPause();
		virtual bool	onBackPressed();

	private:
		void			load();
		void			save();
		void			serialize(Serializer& serializer);
		void			setFadeZoom(float time);

		Audio*			m_pAudio;
		Level			m_level;
		Player			m_player;
		Input			m_input;

		int				m_currentLevel;
		float			m_scale;
		float			m_stateTime;
		float			m_timeLeft;
		float			m_timeIncrement;
		bool			m_pause;
		int				m_hiScore;

		Vector2			m_screenSize;

		GameState		m_gameState;
		GameState		m_nextState;
		bool			m_isInGame;

		Renderer		m_renderer;
	};

	ApplicationBase* newApplication(GameFramework& gameFramework);
}

#endif
