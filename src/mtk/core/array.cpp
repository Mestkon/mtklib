#include <mtk/core/array.hpp>

#include <cstdio>
#include <stdexcept>

namespace mtk {
namespace impl_core {
namespace array {

void _throw_out_of_range_exception(size_t idx, size_t size)
{
	char buf[512];
	std::snprintf(buf, sizeof(buf), "mtk::array::at index out of range (index: %zu, size: %zu)", idx, size);
	throw std::out_of_range(buf);
}

} // namespace array
} // namespace impl_core
} // namespace mtk
