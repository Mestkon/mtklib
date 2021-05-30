#ifndef MTK_CORE_MEM_CAST_HPP
#define MTK_CORE_MEM_CAST_HPP

//! @file
//! Contains mtk::mem_cast

#include <mtk/core/assert.hpp>
#include <mtk/core/preprocessor.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/require.hpp>

#include <cstring>
#include <type_traits>

namespace mtk {
inline namespace casts {

//! @addtogroup core
//! @{

//! @brief Returns the bytes given in from as type U.
//!
//! @code
//! #include <mtk/core/mem_cast.hpp>
//! @endcode
//!
//! Use this instead of reinterpret_cast to avoid UB.
//!
//! @pre U must be default constructible.
//! @pre U must be trivially copyable.
//! @pre T must be trivially copyable.
template<class U
	,class T
#ifndef MTK_DOXYGEN
	,_require<sizeof(U) == sizeof(T)> = 0
	,_require<std::is_default_constructible_v<U>> = 0
	,_require<std::is_trivially_copyable_v<U>> = 0
	,_require<std::is_trivially_copyable_v<T>> = 0
#endif
>
U
mem_cast(const T& from)
noexcept(std::is_nothrow_default_constructible_v<U>)
{
	U ret{ };
	std::memcpy(mtk::_addressof(ret), mtk::_addressof(from), sizeof(U));
	return ret;
}

//! @brief Returns the bytes given as type U.
//!
//! @code
//! #include <mtk/core/mem_cast.hpp>
//! @endcode
//!
//! Use this instead of reinterpret_cast to avoid UB.
//!
//! @pre U must be default constructible.
//! @pre U must be trivially copyable.
//! @pre buffer != nullptr.
//! @pre buffer must be at least buf_len large.
//! @pre buf_len >= sizeof(U).
template<class U
#ifndef MTK_DOXYGEN
	,_require<std::is_default_constructible_v<U>> = 0
	,_require<std::is_trivially_copyable_v<U>> = 0
#endif
>
U
mem_cast(const byte* buffer, size_t buf_len)
noexcept(std::is_nothrow_default_constructible_v<U>)
{
	MTK_ASSERT(buffer);
	MTK_ASSERT(buf_len >= sizeof(U));
	MTK_IGNORE(buf_len);

	U ret{ };
	std::memcpy(mtk::_addressof(ret), buffer, sizeof(U));
	return ret;
}

//! @}

} // namespace casts
} // namespace mtk

#endif
