#ifndef EXO_BASE_TYPES_HPP
#define EXO_BASE_TYPES_HPP

#include <stdint.h>
#include <stddef.h>

namespace exo
{
	typedef unsigned int uint;
	typedef signed int sint;
	typedef int8_t sint8;
	typedef int16_t sint16;
	typedef int32_t sint32;
	typedef int64_t sint64;
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
	typedef size_t MemorySize;

#define nullptr 0
#define EXO_USE_PARAMETER(x) ((void)(x))
}

#endif
