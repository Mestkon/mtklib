#ifndef MTK_RANGES_IMPL_FUNCTOR_STORAGE_HPP
#define MTK_RANGES_IMPL_FUNCTOR_STORAGE_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/impl/move.hpp>

#include <memory>
#include <optional>
#include <type_traits>

namespace mtk {

template<class Func
	,bool C = std::is_copy_constructible_v<Func> && std::is_copy_assignable_v<Func>>
struct _functor_storage
{
	_functor_storage() :
		func(std::nullopt)
	{ }

	template<class F>
	_functor_storage(F&& f) :
		func(mtk::_forward<F>(f))
	{ }

	Func&
	get() const
	{
		MTK_ASSERT(this->func.has_value());
		return *this->func;
	}

	mutable std::optional<Func> func;
};

template<class Func>
struct _functor_storage<Func, false>
{
	_functor_storage() :
		func(nullptr)
	 { }

	template<class F>
	_functor_storage(F&& f) :
		func(std::make_shared<Func>(mtk::_forward<F>(f)))
	{ }

	Func&
	get() const
	{
		MTK_ASSERT(this->func != nullptr);
		return *this->func;
	}

	std::shared_ptr<Func> func;
};

} // namespace mtk

#endif
