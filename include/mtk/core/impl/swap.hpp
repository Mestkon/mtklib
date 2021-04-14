#ifndef MTK_CORE_IMPL_SWAP_HPP
#define MTK_CORE_IMPL_SWAP_HPP

#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,_require<std::is_move_constructible_v<T>> = 0
	,_require<std::is_move_assignable_v<T>> = 0>
constexpr
void
_swap(T& a, T& b)
noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>)
{
	T tmp = mtk::_move(a);
	a = mtk::_move(b);
	b = mtk::_move(tmp);
}

template<class T
	,class U = T
	,_require<std::is_move_constructible_v<T>> = 0
	,_require<std::is_assignable_v<T&, U>> = 0>
constexpr
T
_exchange(T& obj, U&& new_value)
noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_assignable_v<T&, U>)
{
	T old_value = mtk::_move(obj);
	obj = mtk::_forward<U>(new_value);
	return old_value;
}

} // namespace mtk

#endif
