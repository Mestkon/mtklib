#include <mtk/core/nullptr_exception.hpp>

namespace mtk {

void
_throw_nullptr_exception() noexcept(false)
{
	throw nullptr_exception();
}

const char*
nullptr_exception::what() const noexcept
{
	return "nullptr exception";
}

} // namespace mtk
