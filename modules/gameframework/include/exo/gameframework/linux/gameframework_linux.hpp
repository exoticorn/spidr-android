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
		virtual uint		getNumTouches() const { return m_isMouseDown ? 1u : 0u; }
		virtual const Touch&	getTouch(uint) const { return m_touch; }

	private:
		static void			audioCallback(void* pUser, unsigned char* pBuffer, int size);

		SDL_Surface*		m_pScreen;

		bool				m_isMouseDown;
		Touch				m_touch;
	};
}

#endif
