#ifndef MTK_CORE_IMPL_MOVE_HPP
#define MTK_CORE_IMPL_MOVE_HPP

#include <type_traits>

namespace mtk {

template<class T>
constexpr
std::remove_reference_t<T>&&
_move(T&& t) noexcept
{
	return static_cast<std::remove_reference_t<T>&&>(t);
}

template<class T>
constexpr
T&&
_forward(std::remove_reference_t<T>& t) noexcept
{
	return static_cast<T&&>(t);
}

template<class T>
constexpr
T&&
_forward(std::remove_reference_t<T>&& t) noexcept
{
	return static_cast<T&&>(t);
}

} // namespace mtk

#endif
