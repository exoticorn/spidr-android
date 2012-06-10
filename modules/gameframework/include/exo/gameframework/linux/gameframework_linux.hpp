#ifndef EXO_GAMEFRAMEWORK_LINUX_HPP
#define EXO_GAMEFRAMEWORK_LINUX_HPP

struct SDL_Surface;

namespace exo
{
	class GameFrameworkLinux : public GameFramework
	{
	public:
		GameFrameworkLinux();
		~GameFrameworkLinux();

		void				run();

	private:
		SDL_Surface*		m_pScreen;
	};
}

#endif
