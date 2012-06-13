#ifndef EXO_GAMEFRAMEWORK_ANDROID_HPP
#define EXO_GAMEFRAMEWORK_ANDROID_HPP

namespace exo
{
	class GameFrameworkAndroid : public GameFramework
	{
	public:
		GameFrameworkAndroid();
		~GameFrameworkAndroid();

		virtual uint		getNumTouches() const;
		virtual const Touch&	getTouch(uint index) const;

		void				surfaceCreated();
		void				update();
		void				setScreenSize(uint width, uint height);

	private:
		Touch				m_touch;
	};
}

#endif
