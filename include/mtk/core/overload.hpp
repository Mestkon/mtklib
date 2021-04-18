#ifndef MTK_CORE_OVERLOAD_HPP
#define MTK_CORE_OVERLOAD_HPP

#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {
namespace impl_overload {

template<class Func>
struct const_function { };

template<class R
	,class... Args>
struct const_function<R(Args...)>
{
	using type = R(Args...) const;
};

} // namespace impl_overload

template<class Func
#ifndef MTK_DOXYGEN
	,_require<std::is_function_v<Func>> = 0
#endif
>
constexpr
auto
overload(Func* ptr) noexcept
{
	return ptr;
}

template<class Func
	,class T
#ifndef MTK_DOXYGEN
	,_require<std::is_function_v<Func>> = 0
#endif
>
constexpr
auto
overload(Func T::*&& ptr) noexcept
{
	return ptr;
}

template<class Func
	,class T
#ifndef MTK_DOXYGEN
	,_require<std::is_function_v<Func>> = 0
#endif
>
constexpr
auto
overload(typename impl_overload::const_function<Func>::type T::* const& ptr) noexcept
{
	return ptr;
}

} // namespace mtk

#endif
