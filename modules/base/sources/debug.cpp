#include "exo/base/debug.hpp"
#include <stdarg.h>
#include <stdio.h>

namespace exo
{
	void printAndHalt(const char* pFmt, ...)
	{
		va_list va;
		va_start(va, pFmt);
		vprintf(pFmt, va);
		va_end(va);
		__builtin_trap();
	}
}
