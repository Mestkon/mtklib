#ifndef MTK_CORE_MEM_CAST_HPP
#define MTK_CORE_MEM_CAST_HPP

#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/require.hpp>

#include <cstring>
#include <type_traits>

namespace mtk {
inline namespace casts {

template<class U
	,class T
#ifndef MTK_DOXYGEN
	,_require<sizeof(U) == sizeof(T)> = 0
	,_require<std::is_nothrow_default_constructible_v<U>> = 0
	,_require<std::is_trivially_copyable_v<U>> = 0
	,_require<std::is_trivially_copyable_v<T>> = 0
#endif
>
U
mem_cast(const T& from) noexcept
{
	U ret;
	std::memcpy(mtk::_addressof(ret), mtk::_addressof(from), sizeof(U));
	return ret;
}

} // namespace casts
} // namespace mtk

#endif
