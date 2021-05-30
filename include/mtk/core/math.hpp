#ifndef MTK_CORE_MATH_HPP
#define MTK_CORE_MATH_HPP

//! @file
//! Contains basic math functionality

#include <mtk/core/preprocessor.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Contains math constants.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
namespace math {

//! @addtogroup core
//! @{

//! @brief Constant representing π.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
pi = static_cast<T>(3.141592653589793238462643383279502884L);

//! @brief Constant representing e (Eulers' number).
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
e = static_cast<T>(2.718281828459045235360287471352662498L);

//! @brief Constant representing the natural logarithm of 2.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
ln_2 = static_cast<T>(1.414213562373095048801688724209698079L);

//! @brief Constant representing the natural logarithm of 10.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
ln_10 = static_cast<T>(2.302585092994045684017991454684364208L);

//! @brief Constant representing the square root of 2.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
sqrt_2 = static_cast<T>(1.414213562373095048801688724209698079L);

//! @brief Constant representing the square root of π.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
inline constexpr
T
sqrt_pi = static_cast<T>(1.772453850905516027298167483341145182L);

//! @}

} // namespace math



//! @brief Returns val*val.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
template<class T>
constexpr
auto
squared(const T& val)
#ifndef MTK_DOXYGEN
	MTK_RETURNS(val*val)
#else
	;
#endif

//! @brief Returns val*val*val.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
template<class T>
constexpr
auto
cubed(const T& val)
#ifndef MTK_DOXYGEN
	MTK_RETURNS(val*val*val)
#else
	;
#endif



//! @brief Returns the sign of val (-1, 0, 1 formalism).
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
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

//! @brief Returns 0 if val is less than 0, 0.5 if val is 0, or 1 if val is > 0.
//!
//! @code
//! #include <mtk/core/math.hpp>
//! @endcode
//!
//! @pre T must be an arithmetic type.
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

//! @}

} // namespace mtk

#endif
