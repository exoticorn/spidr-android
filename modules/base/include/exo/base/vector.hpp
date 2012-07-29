#ifndef EXO_BASE_VECTOR_HPP
#define EXO_BASE_VECTOR_HPP

#include "exo/base/types.hpp"
#include "exo/base/debug.hpp"

namespace exo
{
	template <typename T>
	class Vector
	{
	public:
		Vector()
		{
			m_pData = new T[8];
			m_capacity = 8;
			m_size = 0;
		}

		~Vector()
		{
			delete [] m_pData;
		}

		void		pushBack(const T& v)
		{
			if(m_size == m_capacity)
			{
				T* pNewData = new T[m_capacity * 2];
				for(uint i = 0; i < m_size; ++i)
				{
					pNewData[i] = m_pData[i];
				}
				delete [] m_pData;
				m_pData = pNewData;
				m_capacity *= 2;
			}
			m_pData[m_size++] = v;
		}

		T&			operator[](uint index)
		{
			ASSERTE(index < m_size);
			return m_pData[index];
		}

		uint		getSize() const { return m_size; }
		const T*	getData() const { return m_pData; }
		T*			getData() { return m_pData; }
	private:
		uint		m_capacity;
		uint		m_size;
		T*			m_pData;
	};
}

#endif
