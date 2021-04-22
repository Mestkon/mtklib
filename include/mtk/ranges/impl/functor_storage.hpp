#ifndef MTK_RANGES_IMPL_FUNCTOR_STORAGE_HPP
#define MTK_RANGES_IMPL_FUNCTOR_STORAGE_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/impl/move.hpp>

#include <memory>
#include <optional>
#include <type_traits>

namespace mtk {

template<class Pred
	,bool C = std::is_copy_constructible_v<Pred> && std::is_copy_assignable_v<Pred>>
struct _functor_storage
{
	_functor_storage() :
		pred(std::nullopt)
	{ }

	template<class P>
	_functor_storage(P&& p) :
		pred(mtk::_forward<P>(p))
	{ }

	Pred&
	get() const
	{
		MTK_ASSERT(this->pred.has_value());
		return *this->pred;
	}

	mutable std::optional<Pred> pred;
};

template<class Pred>
struct _functor_storage<Pred, false>
{
	_functor_storage() :
		pred(nullptr)
	 { }

	template<class P>
	_functor_storage(P&& p) :
		pred(std::make_shared<Pred>(mtk::_forward<P>(p)))
	{ }

	Pred&
	get() const
	{
		MTK_ASSERT(this->pred != nullptr);
		return *this->pred;
	}

	std::shared_ptr<Pred> pred;
};

} // namespace mtk

#endif
