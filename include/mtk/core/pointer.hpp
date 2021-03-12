#ifndef MTK_CORE_POINTER_HPP
#define MTK_CORE_POINTER_HPP

#include <mtk/core/impl/pointer_validators.hpp>

#include <mtk/core/guarded_ptr.hpp>

namespace mtk {

template<class T>
using pointer = guarded_ptr<T, asserting_pointer_validator>;

} // namespace mtk

#endif
