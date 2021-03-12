#include <mtk/core/impl/pointer_validators.hpp>
#include <mtk/core/nullptr_exception.hpp>

namespace mtk {

void
default_pointer_validator::_failure() const
{
	mtk::_throw_nullptr_exception();
}

} // namespace mtk
