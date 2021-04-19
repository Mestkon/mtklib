#include <mtk/core/scope_exit.hpp>

#include <exception>

namespace mtk {
namespace impl_scope_exit {

bool
_uncaught_exceptions() noexcept
{
	return (std::uncaught_exceptions() > 0);
}

} // namespace impl_scope_exit
} // namespace mtk
