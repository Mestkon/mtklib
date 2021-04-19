#include <mtk/core/scope_guard.hpp>

#include <exception>

namespace mtk {
namespace impl_scope_guard {

bool
_uncaught_exceptions() noexcept
{
	return (std::uncaught_exceptions() > 0);
}

} // namespace impl_scope_guard
} // namespace mtk
