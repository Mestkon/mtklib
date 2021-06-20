#ifndef MTK_CORE_NARROW_CAST_HPP
#define MTK_CORE_NARROW_CAST_HPP

//! @file
//! Contains mtk::narrow_cast

#include <mtk/core/saturate_cast.hpp>
#include <mtk/core/impl/require.hpp>

#include <exception>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Thrown by mtk::narrow_cast.
//!
//! @code
//! #include <mtk/core/narrow_cast.hpp>
//! @endcode
struct narrowing_exception :
	std::exception
{
	const char*
	what() const noexcept override;
};

[[noreturn]]
void
_throw_narrowing_exception();



inline namespace casts {

//! @addtogroup core
//! @{

//! @brief Returns static_cast<U>(val), throws exception if narrowing occured.
//!
//! @code
//! #include <mtk/core/narrow_cast.hpp>
//! @endcode
//!
//! Throws mtk::narrowing_exception if val can not be represented
//! in the type U.
//!
//! @pre U must be arithmetic.
//! @pre T must be arithmetic.
template<class U
	,class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<U>> = 0
	,_require<std::is_arithmetic_v<T>> = 0
#endif
>
constexpr
U
narrow_cast(T val)
{
	if (static_cast<T>(mtk::saturate_cast<U>(val) != val))
		mtk::_throw_narrowing_exception();

	return static_cast<U>(val);
}

//! @}

} // namespace casts

//! @}

} // namespace mtk


#endif
