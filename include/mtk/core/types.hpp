#ifndef MTK_CORE_TYPES_HPP
#define MTK_CORE_TYPES_HPP

//! @file
//! Contains useful short typedefs.

#include <cstddef>
#include <cstdint>

namespace mtk {

//! @addtogroup core
//! @{

inline namespace types {

//! @addtogroup core
//! @{

//! Typedef.
using llong = long long;
//! Typedef.
using ldouble = long double;

//! Typedef.
using schar = signed char;

//! Typedef.
using uchar = unsigned char;
//! Typedef.
using ushort = unsigned short;
//! Typedef.
using uint = unsigned int;
//! Typedef.
using ulong = unsigned long;
//! Typedef.
using ullong = unsigned long long;

//! Typedef.
using byte = uchar;

//! Typedef.
using std::size_t;
//! Typedef.
using std::ptrdiff_t;
//! Typedef.
using std::intptr_t;
//! Typedef.
using std::uintptr_t;

#if (defined(INT8_MAX) && defined(INT16_MAX) && defined(INT32_MAX) && defined(INT64_MAX) && \
	defined(UINT8_MAX) && defined(UINT16_MAX) && defined(UINT32_MAX) && defined(UINT64_MAX)) || \
	defined(MTK_DOXYGEN)

	//! @brief Defined if (u)intN_t types are available.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	#define MTK_FIXED_WIDTH_INTS

	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using int8_t = std::int8_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using int16_t = std::int16_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using int32_t = std::int32_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using int64_t = std::int64_t;

	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using uint8_t = std::uint8_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using uint16_t = std::uint16_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using uint32_t = std::uint32_t;
	//! Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	using uint64_t = std::uint64_t;

#endif

//! @}

} // namespace types

//! @}

} // namespace mtk

#endif
