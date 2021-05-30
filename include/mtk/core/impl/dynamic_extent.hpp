#ifndef MTK_CORE_IMPL_DYNAMIC_EXTENT_HPP
#define MTK_CORE_IMPL_DYNAMIC_EXTENT_HPP

#include <mtk/core/types.hpp>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Declares an extent or size as dynamic.
//!
//! @code
//! #include <mtk/core/array.hpp>
//! #include <mtk/linalg/fwd.hpp>
//! #inlcude <mtk/linalg/matrix.hpp>
//! @endcode
inline constexpr
size_t
dynamic_extent = static_cast<size_t>(-1);

//! @}

} // namespace mtk

#endif
