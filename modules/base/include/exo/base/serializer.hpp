#ifndef EXO_SERIALIZER_HPP
#define EXO_SERIALIZER_HPP

#include "exo/base/types.hpp"

namespace exo
{
	class Serializer
	{
	public:
		Serializer(uint8 dataVersion);	// write constructor
		Serializer(const void* pBuffer, uint size);	// read constructor
		~Serializer();

		bool			isValid() const;

		const uint8*	getData(uint* pSize);	// returns new buffer. Application needs to delete this later.
		uint8			getDataVersion() const { return m_pBuffer[0]; }

		bool			isReading() const { return m_isReading; }

		template <typename T>
		void			serialize(T* pValue) { serialize(pValue, sizeof(T)); }

		void			serialize(void* pData, uint size);

	private:
		bool			m_isReading;
		uint8*			m_pBuffer;
		uint			m_position;
		uint			m_bufferSize;
	};
}

#endif
