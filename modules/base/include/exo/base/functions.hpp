#ifndef EXO_BASE_FUNCTIONS_HPP
#define EXO_BASE_FUNCTIONS_HPP

namespace exo
{
	template <typename T>
	T max(T a, T b) { return a > b ? a : b; }

	template <typename T>
	T min(T a, T b) { return a < b ? a : b; }

	template <typename T>
	T abs(T v) { return v < 0 ? -v : v; }

	template <typename T>
	T clamp(T a, T b, T v) { return v < a ? a : (v > b ? b : v); }
}

#endif
