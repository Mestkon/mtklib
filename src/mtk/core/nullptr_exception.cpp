#include <mtk/core/nullptr_exception.hpp>

namespace mtk {

const char*
nullptr_exception::what() const noexcept
{
	return "nullptr exception";
}

void
_throw_nullptr_exception()
{
	throw nullptr_exception();
}



} // namespace mtk
