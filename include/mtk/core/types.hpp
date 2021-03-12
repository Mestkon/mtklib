#ifndef MTK_CORE_TYPES_HPP
#define MTK_CORE_TYPES_HPP

#include <cstddef>
#include <cstdint>

namespace mtk {
inline namespace types {

using llong = long long;
using ldouble = long double;

using schar = signed char;

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ullong = unsigned long long;

using byte = uchar;

using std::size_t;
using std::ptrdiff_t;
using std::intptr_t;
using std::uintptr_t;

#if (defined(INT8_MAX) && defined(INT16_MAX) && defined(INT32_MAX) && defined(INT64_MAX) && \
	defined(UINT8_MAX) && defined(UINT16_MAX) && defined(UINT32_MAX) && defined(UINT64_MAX)) || \
	defined(MTK_DOXYGEN)

	//! Defined if (u)intN_t types are available
	#define MTK_FIXED_WIDTH_INTS

	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::int64_t;

	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;

#endif

} // namespace types

} // namespace mtk

#endif
