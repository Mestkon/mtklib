#ifndef MTK_CORE_NULLPTR_EXCEPTION_HPP
#define MTK_CORE_NULLPTR_EXCEPTION_HPP

#include <exception>

namespace mtk {

class nullptr_exception :
	public std::exception
{
public:
	const char*
	what() const noexcept override;
};

[[noreturn]]
void
_throw_nullptr_exception();

} // namespace mtk

#endif
