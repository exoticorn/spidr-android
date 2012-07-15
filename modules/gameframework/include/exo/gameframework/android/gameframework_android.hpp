#ifndef EXO_GAMEFRAMEWORK_ANDROID_HPP
#define EXO_GAMEFRAMEWORK_ANDROID_HPP

#include "exo/gameframework/gameframework.hpp"

namespace exo
{
	class GameFrameworkAndroid : public GameFramework
	{
	public:
		GameFrameworkAndroid(const char* pStoragePath);
		~GameFrameworkAndroid();

		virtual uint		getNumTouches() const;
		virtual const Touch&	getTouch(uint index) const;

		void				surfaceCreated();
		void				update();
		void				setScreenSize(uint width, uint height);
		void				handleTouchInput(int id, bool down, float x, float y);

	private:
		Touch				m_touch;
		bool				m_hasTouch;
	};
}

#endif
