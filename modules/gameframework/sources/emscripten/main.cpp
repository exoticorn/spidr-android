#include "exo/gameframework/gameframework.hpp"
#include <emscripten.h>

static exo::GameFrameworkEmscripten* s_pGameFramework;

void runOneFrame()
{
	s_pGameFramework->run();
}

int main()
{
	s_pGameFramework = new exo::GameFrameworkEmscripten();
	
	emscripten_set_main_loop(runOneFrame, 60, true);
}
