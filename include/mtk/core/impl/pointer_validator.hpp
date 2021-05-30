#ifndef MTK_CORE_IMPL_POINTER_VALIDATOR_HPP
#define MTK_CORE_IMPL_POINTER_VALIDATOR_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/nullptr_exception.hpp>

namespace mtk {

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

struct asserting_pointer_validator
{
	constexpr
	void
	operator()([[maybe_unused]] const void* p) const
	{
		MTK_ASSERT(p);
	}
};

struct empty_pointer_validator
{
	constexpr
	void
	operator()(const void*) const
	{ }
};

} // namespace mtk

#endif
