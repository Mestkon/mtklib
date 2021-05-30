#include <mtk/core/impl/exception.hpp>

#include <exception>

namespace mtk {

bool
_uncaught_exceptions() noexcept
{
	return (std::uncaught_exceptions() > 0);
}

} // namespace mtk
