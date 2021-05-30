#ifndef MTK_CORE_NULLPTR_EXCEPTION_HPP
#define MTK_CORE_NULLPTR_EXCEPTION_HPP

//! @file
//! Contains mtk::nullptr_exception

#include <exception>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Thrown by mtk::default_pointer_validator if given pointer is nullptr.
//!
//! @code
//! #include <mtk/core/nullptr_exception.hpp>
//! @endcode
class nullptr_exception :
	public std::exception
{
public:
	const char*
	what() const noexcept override;
};

//! @}

[[noreturn]]
void
_throw_nullptr_exception();

} // namespace mtk

#endif
