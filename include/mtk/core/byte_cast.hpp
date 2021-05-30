#ifndef MTK_CORE_BYTE_CAST_HPP
#define MTK_CORE_BYTE_CAST_HPP

//! @file
//! Contains mtk::byte_cast

#include <mtk/core/types.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {
inline namespace casts {

//! @addtogroup core
//! @{

//! @brief Returns val reinterpret_casted to a corresponding
//! cv-qualified pointer to mtk::byte.
//!
//! @code
//! #include <mtk/core/byte_cast.hpp>
//! @endcode
//!
//! @pre std::remove_cv_t<T> must be trivially copyable or void.
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_trivially_copyable_v<std::remove_cv_t<T>>
		|| std::is_void_v<std::remove_cv_t<T>>> = 0
#endif
>
auto
byte_cast(T* val) noexcept
{
	using vbyte = std::conditional_t<std::is_volatile_v<T>, volatile byte, byte>;
	using cvbyte = std::conditional_t<std::is_const_v<T>, const vbyte, vbyte>;
	return reinterpret_cast<cvbyte*>(val);
}

//! @}

} // namespace casts
} // namespace mtk

#endif
