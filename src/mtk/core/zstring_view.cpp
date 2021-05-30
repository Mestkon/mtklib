#include <mtk/core/zstring_view.hpp>

#include <iostream>
#include <string_view>

namespace mtk {

bool
operator==(zstring_view lhs, zstring_view rhs) noexcept
{
	if (lhs.size() != rhs.size())
		return false;

	if (!lhs.data() && !rhs.data())
		return true;

	if (bool(lhs.data()) ^ bool(rhs.data()))
		return false;

	return (std::strcmp(lhs.data(), rhs.data()) == 0);
}

bool
operator<(zstring_view lhs, zstring_view rhs) noexcept
{
	if (!lhs.data() && !rhs.data())
		return false;

	if (!lhs.data() && rhs.data())
		return true;

	if (lhs.data() && !rhs.data())
		return false;

	return (std::strcmp(lhs.data(), rhs.data()) < 0);
}

std::ostream&
operator<<(std::ostream& os, zstring_view zv)
{
	os << std::string_view(zv);
	return os;
}

} // namespace mtk
