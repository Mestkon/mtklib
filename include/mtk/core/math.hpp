#ifndef MTK_CORE_MATH_HPP
#define MTK_CORE_MATH_HPP

#include <mtk/core/preprocessor.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {
namespace math {

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
pi = static_cast<T>(3.141592653589793238462643383279502884L);

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
e = static_cast<T>(2.718281828459045235360287471352662498L);

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
ln_2 = static_cast<T>(1.414213562373095048801688724209698079L);

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
ln_10 = static_cast<T>(2.302585092994045684017991454684364208L);

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
sqrt_2 = static_cast<T>(1.414213562373095048801688724209698079L);

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
sqrt_pi = static_cast<T>(1.772453850905516027298167483341145182L);

} // namespace math



template<class T>
constexpr
auto
squared(const T& val) MTK_RETURNS(val*val)

template<class T>
constexpr
auto
cubed(const T& val) MTK_RETURNS(val*val*val)



template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
constexpr
T
sgn(T val) noexcept
{
	return T(int(T() < val) - int(val < T()));
}

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
constexpr
T
heaviside(T val) noexcept
{
	return T((mtk::sgn(val) + T(1)) / T(2));
}

} // namespace mtk

#endif
