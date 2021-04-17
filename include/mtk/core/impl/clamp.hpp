#ifndef MTK_CORE_IMPL_CLAMP_HPP
#define MTK_CORE_IMPL_CLAMP_HPP

#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,_require<std::is_arithmetic_v<T>> = 0>
constexpr
T
_min(T a, T b) noexcept
{
	return (b < a ? b : a);
}

template<class T
	,_require<std::is_arithmetic_v<T>> = 0>
constexpr
T
_max(T a, T b) noexcept
{
	return (a < b ? b : a);
}

template<class T
	,_require<std::is_arithmetic_v<T>> = 0>
constexpr
T
_clamp(T v, T lo, T hi) noexcept
{
	return (v < lo ? lo : (hi < v ? hi : v));
}

} // namespace mtk

#endif
