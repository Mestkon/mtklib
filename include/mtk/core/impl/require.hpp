#ifndef MTK_CORE_IMPL_REQUIRE_HPP
#define MTK_CORE_IMPL_REQUIRE_HPP

#include <type_traits>

namespace mtk {

template<bool Cond>
using _require = std::enable_if_t<Cond, int>;

} // namespace mtk

#endif
