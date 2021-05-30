#ifndef MTK_CORE_OVERLOAD_HPP
#define MTK_CORE_OVERLOAD_HPP

//! @file
//! Contains mtk::overload

#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

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

//! @brief Returns a pointer to the selected function overload.
//!
//! @code
//! #include <mtk/core/overload.hpp>
//! @endcode
//!
//! Examples:
//! @code
//! void f(); // 1
//! int f(int); // 2
//!
//! struct S
//! {
//!		int f(); // 3
//!		int f() const; // 4
//!		float f(float) const; // 5
//!		double f(double); // 6
//! };
//!
//! int main()
//! {
//!		auto p1 = mtk::overload<void()>(&f); // selects 1
//!		auto p2 = mtk::overload<int(int)>(&f); // selects 2
//!
//!		auto p3 = mtk::overload<int()>(&S::f); // selects 3
//!		auto p4 = mtk::overload<int() const>(&S::f); // selects 4
//!		auto p5 = mtk::overload<float(float)>(&S::f); // selects 5
//!		auto p6 = mtk::overload<double(double)>(&S::f); // selects 6
//! }
//! @endcode
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

//! @}

} // namespace mtk

#endif
