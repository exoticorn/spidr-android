#include "exo/base/timer.hpp"
#include "exo/base/types.hpp"

#ifdef EXO_PLATFORM_EMSCRIPTEN
#include <SDL/SDL.h>
#endif

namespace exo
{
	Timer::Timer()
	{
		reset();
	}

	void Timer::reset()
	{
#ifndef EXO_PLATFORM_EMSCRIPTEN
		clock_gettime(CLOCK_MONOTONIC, &m_lastTime);
#else
		m_lastTime = SDL_GetTicks();
#endif
	}

	float Timer::getElapsedTime() const
	{
#ifndef EXO_PLATFORM_EMSCRIPTEN
		timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		uint64 delta = (now.tv_nsec - m_lastTime.tv_nsec) + (now.tv_sec - m_lastTime.tv_sec) * 1000000000ull;
		return delta / 1000000000.0f;
#else
		uint32 now = SDL_GetTicks();
		uint32 delta = now - m_lastTime;
		return delta / 1000.0f;
#endif
	}
}
