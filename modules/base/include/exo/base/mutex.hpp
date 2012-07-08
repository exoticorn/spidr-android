#ifndef EXO_MUTEX_HPP
#define EXO_MUTEX_HPP

#include <pthread.h>

namespace exo
{
	class Mutex
	{
	public:
		Mutex();
		~Mutex();

		void	lock();
		void	unlock();
	private:
		pthread_mutex_t	m_mutex;
	};
}

#endif
