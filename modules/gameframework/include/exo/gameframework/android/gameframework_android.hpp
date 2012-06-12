#ifndef EXO_GAMEFRAMEWORK_ANDROID_HPP
#define EXO_GAMEFRAMEWORK_ANDROID_HPP

namespace exo
{
	class GameFrameworkAndroid : public GameFramework
	{
	public:
		GameFrameworkAndroid();
		~GameFrameworkAndroid();

		void				surfaceCreated();
		void				update();
		void				setScreenSize(uint width, uint height);
	};
}

#endif
