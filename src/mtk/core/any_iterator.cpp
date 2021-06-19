#include <mtk/core/any_iterator.hpp>

namespace mtk {

const char*
bad_any_iterator_operation::what() const noexcept
{
	return "bad any_iterator operation";
}

void _throw_bad_any_iterator_operation()
{
	throw bad_any_iterator_operation();
}

} // namespace mtk

