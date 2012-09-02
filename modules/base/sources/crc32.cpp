#include "exo/base/crc32.hpp"
#include "zlib.h"

namespace exo
{
	uint32 getCRC32(const void* pBuffer, uint size)
	{
		return crc32(crc32(0, nullptr, 0), (const Bytef*)pBuffer, size);
	}
}
