#ifndef MTK_RANGES_GENERATE_HPP
#define MTK_RANGES_GENERATE_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/ranges/impl/functor_storage.hpp>

#include <iterator>
#include <optional>
#include <type_traits>

namespace mtk {
namespace impl_generate {

template<class Func>
class _generator_iterator
{
public:
	using value_type = std::decay_t<decltype(mtk::_declval<Func&>()())>;
	using reference = value_type;
	using pointer = struct {
		reference value;
		auto
		operator->() const { return mtk::_addressof(this->value); }
	};
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	_generator_iterator() :
		m_func(),
		m_value(std::nullopt)
	{ }

	template<class F
		,_require<std::is_constructible_v<Func, F>> = 0
	>
	_generator_iterator(F&& f) :
		m_func(mtk::_forward<F>(f)),
		m_value(std::nullopt)
	{
		++*this;
	}

	_generator_iterator(value_type val) :
		m_func(),
		m_value(mtk::_move(val))
	{ }

	reference
	operator*() const
	{
		MTK_ASSERT(m_value.has_value());
		return *m_value;
	}

	pointer
	operator->() const
	{
		return pointer{**this};
	}

	friend
	_generator_iterator&
	operator++(_generator_iterator& rhs)
	{
		rhs.m_value = rhs.m_func.get()();
		return rhs;
	}

	friend
	_generator_iterator
	operator++(_generator_iterator& lhs, int)
	{
		MTK_ASSERT(lhs.m_value.has_value());
		_generator_iterator cp(*lhs.m_value);
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const _generator_iterator&, const _generator_iterator&)
	{
		return false;
	}

	friend
	bool
	operator!=(const _generator_iterator&, const _generator_iterator&)
	{
		return true;
	}

private:
	_functor_storage<Func> m_func;
	std::optional<value_type> m_value;
};

} // namespace impl_generate



template<class Func
#ifndef MTK_DOXYGEN
	,_require<std::is_invocable_v<std::decay_t<Func>>> = 0
#endif
>
auto
generate(Func&& f)
{
	impl_generate::_generator_iterator<std::decay_t<Func>> first(mtk::_forward<Func>(f));
	impl_generate::_generator_iterator<std::decay_t<Func>> last;
	return mtk::range(first, last);
}

} // namespace mtk

#endif
