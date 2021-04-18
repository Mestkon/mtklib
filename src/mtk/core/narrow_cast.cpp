#include <mtk/core/narrow_cast.hpp>

namespace mtk {

const char*
narrowing_exception::what() const noexcept
{
	return "narrowing exception";
}

void
_throw_narrowing_exception()
{
	throw narrowing_exception();
}

} // namespace mtk
