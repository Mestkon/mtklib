#ifndef MTK_CORE_PREPROCESSOR_HPP
#define MTK_CORE_PREPROCESSOR_HPP

//! @file
//! Contains common preprocessor macros.

namespace mtk {

//! @addtogroup core
//! @{

namespace impl_core {
namespace preprocessor {

template<class... Args>
constexpr
void
_no_op(Args&& ...) noexcept { }

} // namespace preprocessor
} // namespace impl_core

//! @brief Evaluates to a no-op.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
#define MTK_NO_OP mtk::_no_op()

//! @brief Evaluates the given comma separated expressions and discards the results.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
//!
//! Can be used to mark parameters as unused.
#define MTK_IGNORE(...) mtk::impl_core::preprocessor::_no_op(__VA_ARGS__)

//! @brief Wraps the given texts in quotation marks.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
#define MTK_STRINGIFY_TEXT(X) #X

//! @brief Same as MTK_STRINGIFY_TEXT but evaluates any macros before wrapping.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
#define MTK_STRINGIFY(X) MTK_STRINGIFY_TEXT(X)

//! @brief Concatenates the given text.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
#define MTK_CONCAT_TEXT(A, B) A##B

//! @brief Same as MTK_CONCAT_TEXT but evaluates any macros before concatenation.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
#define MTK_CONCAT(A, B) MTK_CONCAT_TEXT(A, B)

//! @brief Defines noexcept, return type and body of any single expression function.
//!
//! @code
//! #include <mtk/core/preprocessor.hpp>
//! @endcode
//!
//! Example
//! @code
//! auto square(int x) MTK_RETURNS(x*x)
//! auto square2(int x) noexcept(noexcept(x*x)) -> decltype(x*x) { return x*x; }
//! // square and square2 are equivalent.
//! @endcode
#define MTK_RETURNS(X) noexcept(noexcept(X)) -> decltype(X) { return X; }

//! @}

} // namespace mtk

#endif
