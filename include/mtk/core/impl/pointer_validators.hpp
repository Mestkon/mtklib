#ifndef MTK_CORE_IMPL_POINTER_VALIDATORS_HPP
#define MTK_CORE_IMPL_POINTER_VALIDATORS_HPP

#include <mtk/core/assert.hpp>

namespace mtk {

struct default_pointer_validator
{
	void
	_failure() const;

	constexpr
	void
	operator()(const void* p) const
	{
		if (!p)
			this->_failure();
	}
};

struct asserting_pointer_validator
{
	constexpr
	void
	operator()(const void* p) const noexcept
	{
		MTK_ASSERT(p);
	}
};

} // namespace mtk

#endif
