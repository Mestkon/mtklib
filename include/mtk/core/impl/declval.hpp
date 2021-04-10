#ifndef MTK_CORE_DECLVAL_HPP
#define MTK_CORE_DECLVAL_HPP

#include <type_traits>

namespace mtk {

template<class T>
std::add_rvalue_reference_t<T>
_declval() noexcept;

} // namespace mtk

#endif
