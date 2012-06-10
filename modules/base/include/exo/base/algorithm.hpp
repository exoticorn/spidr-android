#ifndef EXO_ALGORITHM_HPP
#define EXO_ALGORITHM_HPP

namespace exo
{
	template <class R, typename T>
	R find(R it, T value) {
		while(!it.isAtEnd() && *it != value) {
			++it;
		}
		return it;
	}
}

#endif // _ALGORITHM_HPP
