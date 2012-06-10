#ifndef EXO_ASSERT_HPP
#define EXO_ASSERT_HPP

#ifndef ASSERTE
#	ifdef BUILD_DEBUG
#		define ASSERTE(expr) if(!(expr)) exo::printAndHalt("Assertion '%s' failed:\n%s:%d\n"#expr, __FILE__, __LINE__); else
#	else
#		define ASSERTE(expr)
#	endif
#endif

#ifndef BREAK
#	ifdef BUILD_DEBUG
#		define BREAK exo::printAndHalt
#	else
#		define BREAK if(0) exo::printAndHalt
#	endif
#endif

namespace exo
{
	void printAndHalt(const char* pFmt, ...);
}

#endif
