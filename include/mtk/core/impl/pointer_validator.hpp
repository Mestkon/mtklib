#ifndef MTK_CORE_IMPL_POINTER_VALIDATOR_HPP
#define MTK_CORE_IMPL_POINTER_VALIDATOR_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/nullptr_exception.hpp>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Use as pointer validator to throw nullptr_exception
//! if a nullptr is dereferenced.
//!
//! @code
//! #include <mtk/core/not_null.hpp>
//! #include <mtk/core/pointer.hpp>
//! #include <mtk/core/unique_ptr.hpp>
//! @endcode
struct default_pointer_validator
{
	constexpr
	void
	operator()(const void* p) const
	{
		if (!p)
			mtk::_throw_nullptr_exception();
	}
};

//! @brief Use as pointer validator to assert that
//! a nullptr is not dereferenced in debug mode.
//!
//! @code
//! #include <mtk/core/not_null.hpp>
//! #include <mtk/core/pointer.hpp>
//! #include <mtk/core/unique_ptr.hpp>
//! @endcode
struct asserting_pointer_validator
{
	constexpr
	void
	operator()([[maybe_unused]] const void* p) const
	{
		MTK_ASSERT(p);
	}
};

//! @brief Use as pointer validator to not validate anything.
//!
//! @code
//! #include <mtk/core/not_null.hpp>
//! #include <mtk/core/pointer.hpp>
//! #include <mtk/core/unique_ptr.hpp>
//! @endcode
struct empty_pointer_validator
{
	constexpr
	void
	operator()(const void*) const
	{ }
};

//! @}

} // namespace mtk

#endif
