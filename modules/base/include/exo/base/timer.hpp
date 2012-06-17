#ifndef EXO_BASE_TIMER_HPP
#define EXO_BASE_TIMER_HPP

#include <time.h>

namespace exo
{
	class Timer
	{
	public:
		Timer();

		void		reset();
		float		getElapsedTime() const;

	private:
		timespec	m_lastTime;
	};
}

#endif
