#include "exo/base/timer.hpp"
#include "exo/base/types.hpp"

namespace exo
{
	Timer::Timer()
	{
		reset();
	}

	void Timer::reset()
	{
		clock_gettime(CLOCK_MONOTONIC, &m_lastTime);
	}

	float Timer::getElapsedTime() const
	{
		timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		uint64 delta = (now.tv_nsec - m_lastTime.tv_nsec) + (now.tv_sec - m_lastTime.tv_sec) * 1000000000ull;
		return delta / 1000000000.0f;
	}
}
