#ifndef MTK_CORE_IMPL_ADDRESSOF_HPP
#define MTK_CORE_IMPL_ADDRESSOF_HPP

#include <mtk/core/types.hpp>

#include <type_traits>

namespace mtk {

template<class T>
T*
_addressof(T& ref) noexcept
{
	return reinterpret_cast<T*>(
		const_cast<byte*>(
		&reinterpret_cast<const volatile byte&>(ref)));
}

} // namespace mtk

#endif
