#ifndef MTK_CORE_NARROW_CAST_HPP
#define MTK_CORE_NARROW_CAST_HPP

#include <mtk/core/saturate_cast.hpp>
#include <mtk/core/impl/require.hpp>

#include <stdexcept>
#include <type_traits>

namespace mtk {

struct narrowing_exception :
	std::exception
{
	const char*
	what() const noexcept override;
};

[[noreturn]]
void
_throw_narrowing_exception();

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

} // namespace mtk

#endif
