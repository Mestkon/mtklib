#ifndef MTK_CORE_NULLPTR_EXCEPTION_HPP
#define MTK_CORE_NULLPTR_EXCEPTION_HPP

#include <exception>

namespace mtk {

[[noreturn]]
void
_throw_nullptr_exception() noexcept(false);



struct nullptr_exception :
	std::exception
{
	const char* what() const noexcept override;
};

} // namespace mtk

#endif
