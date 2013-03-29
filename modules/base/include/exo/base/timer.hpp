#ifndef EXO_BASE_TIMER_HPP
#define EXO_BASE_TIMER_HPP

#ifndef EXO_PLATFORM_EMSCRIPTEN
#include <time.h>
#else
#include "exo/base/types.hpp"
#endif

namespace exo
{
	class Timer
	{
	public:
		Timer();

		void		reset();
		float		getElapsedTime() const;

	private:
#ifndef EXO_PLATFORM_EMSCRIPTEN
		timespec	m_lastTime;
#else
		uint32		m_lastTime;
#endif
	};
}

#endif
