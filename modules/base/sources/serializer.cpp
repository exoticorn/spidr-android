#include "exo/base/serializer.hpp"
#include "exo/base/debug.hpp"
#include <string.h>

namespace exo
{
	Serializer::Serializer(uint8 dataVersion)
	{
		const int initialBufferSize = 64;
		m_pBuffer = new uint8[initialBufferSize];
		m_bufferSize = initialBufferSize;
		m_pBuffer[0] = dataVersion;
		m_position = 9;
		m_isReading = false;
	}

	Serializer::Serializer(const void* pBuffer, uint size)
	{
		m_pBuffer = reinterpret_cast<uint8*>(const_cast<void*>(pBuffer)); // I now what I'm doing. #famouslastwords
		m_bufferSize = size;
		m_position = 9;
		m_isReading = true;
	}

	Serializer::~Serializer()
	{
		if(!m_isReading)
		{
			delete [] m_pBuffer;
		}
	}

	bool Serializer::isValid() const
	{
		uint32 size;
		memcpy(&size, m_pBuffer + 1, 4);
		if(size != m_bufferSize)
		{
			return false;
		}

		// TODO: check hash

		return true;
	}

	const uint8* Serializer::getData(uint* pSize)
	{
		uint32 size = (uint32)m_position;
		memcpy(m_pBuffer + 1, &size, 4);
		m_bufferSize = m_position;

		// TODO: generate hash

		ASSERTE(isValid());

		uint8* pBuffer = new uint8[m_bufferSize];
		memcpy(pBuffer, m_pBuffer, m_bufferSize);
		*pSize = m_bufferSize;
		return pBuffer;
	}

	void Serializer::serialize(void* pValue, uint size)
	{
		if(m_isReading)
		{
			if(m_position + size > m_bufferSize)
			{
				BREAK("Buffer overflow while reading in Serializer");
				memset(pValue, 0, size);
			}
			else
			{
				memcpy(pValue, m_pBuffer + m_position, size);
			}
		}
		else
		{
			while(m_bufferSize < m_position + size)
			{
				uint8* pNewBuffer = new uint8[m_bufferSize * 2];
				memcpy(pNewBuffer, m_pBuffer, m_position);
				delete [] m_pBuffer;
				m_pBuffer = pNewBuffer;
				m_bufferSize *= 2;
			}

			memcpy(m_pBuffer + m_position, pValue, size);
		}
		m_position += size;
	}
}
