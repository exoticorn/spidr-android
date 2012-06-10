#include "exo/gameframework/gameframework.hpp"
#ifdef EXO_BUILD_UNITTEST
#	include "exo/unittest.hpp"
#endif

int main()
{
#ifndef EXO_BUILD_UNITTEST
	exo::GameFrameworkLinux gameFramework;

	gameFramework.run();
#else
	UnitTest::run();
#endif
}
