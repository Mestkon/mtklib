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

//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using llong = long long;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using ldouble = long double;

//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using schar = signed char;

//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using uchar = unsigned char;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using ushort = unsigned short;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using uint = unsigned int;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using ulong = unsigned long;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using ullong = unsigned long long;

//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using byte = uchar;

//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using std::size_t;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using std::ptrdiff_t;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
using std::intptr_t;
//! @brief typedef.
//!
//! @code
//! #include <mtk/core/types.hpp>
//! @endcode
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

	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using int8_t = std::int8_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using int16_t = std::int16_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using int32_t = std::int32_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using int64_t = std::int64_t;

	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using uint8_t = std::uint8_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using uint16_t = std::uint16_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using uint32_t = std::uint32_t;
	//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INTS.
	//!
	//! @code
	//! #include <mtk/core/types.hpp>
	//! @endcode
	using uint64_t = std::uint64_t;

	#if (defined(__GNUC__) && defined(__SIZEOF_INT128__)) || \
		defined(MTK_DOXYGEN)

		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"

		//! @brief Defined if int128_t and uint128_t is available.
		//!
		//! @code
		//! #include <mtk/core/types.h>
		//! @endcode
		#define MTK_FIXED_WIDTH_INT128

		//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INT128.
		//!
		//! @code
		//! #include <mtk/core/types.h>
		//! @endcode
		using int128_t = __int128;
		//! @brief Import into mtk. See @ref MTK_FIXED_WIDTH_INT128.
		//!
		//! @code
		//! #include <mtk/core/types.h>
		//! @endcode
		using uint128_t = unsigned __int128;

		#pragma GCC diagnostic pop
	#endif

#endif

//! @}

} // namespace types

//! @}

} // namespace mtk

#endif
