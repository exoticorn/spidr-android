#include "exo/base/crc32.hpp"
#include "zlib.h"

namespace exo
{
	uint32 getCRC32(const void* pBuffer, uint size)
	{
		EXO_USE_PARAMETER(pBuffer);
		EXO_USE_PARAMETER(size);
#ifndef EXO_PLATFORM_EMSCRIPTEN
		return crc32(crc32(0, nullptr, 0), (const Bytef*)pBuffer, size);
#else
		return 0;
#endif
	}
}
