#ifndef MTK_CORE_IMPL_ALGORITHM_HPP
#define MTK_CORE_IMPL_ALGORITHM_HPP

#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/swap.hpp>

namespace mtk {

template<class Iter
	,class Iter2>
constexpr
void
_copy_range(Iter first, Iter last, Iter2 dst)
{
	while (first != last) {
		*(dst++) = *(first++);
	}
}

template<class Iter
	,class Iter2>
constexpr
void
_move_range(Iter first, Iter last, Iter2 dst)
{
	while (first != last) {
		*(dst++) = mtk::_move(*(first++));
	}
}

template<class Iter>
constexpr
void
_swap_range(Iter first1, Iter last1, Iter first2)
{
	while (first1 != last1) {
		mtk::_swap(*(first1++), *(first2++));
	}
}

template<class Iter>
constexpr
bool
_equal_range(Iter first1, Iter last1, Iter first2)
{
	while (first1 != last1) {
		if (!(*(first1++) == *(first2++)))
			return false;
	}

	return true;
}

template<class Iter>
constexpr
bool
_less_than_range(Iter first1, Iter last1, Iter first2, Iter last2)
{
	while ((first1 != last1) && (first2 != last2)) {
		if (*first1 < *first2)
			return true;
		else if (*first2 < *first1)
			return false;

		++first1;
		++first2;
	}

	return ((first1 == last1) && (first2 != last2));
}

} // namespace mtk

#endif
