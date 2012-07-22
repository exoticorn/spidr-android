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

	class MutexLock
	{
	public:
		MutexLock(Mutex& mutex);
		~MutexLock();

	private:
		Mutex&	m_mutex;
	};
}

#endif
