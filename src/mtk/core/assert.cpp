#include <mtk/core/assert.hpp>

#include <exception>
#include <cstdio>

namespace mtk {

void
_assertion_handler(const char* what, const char* file, int line) noexcept
{
	std::fprintf(stderr, "Assertion failure at %s:%d: %s\n", file, line, what);
	std::fflush(stderr);
	std::terminate();
}

} // namespace mtk
