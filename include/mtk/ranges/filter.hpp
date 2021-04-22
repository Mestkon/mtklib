#ifndef MTK_RANGES_FILTER_HPP
#define MTK_RANGES_FILTER_HPP

#include <mtk/core/iterator_traits.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/ranges/impl/functor_storage.hpp>

#include <iterator>
#include <optional>
#include <type_traits>

namespace mtk {
namespace impl_filter {

template<class Iter
	,class Pred>
decltype(auto)
_increment(Iter& iter, Iter& end, Pred& p)
{
	while (++iter != end) {
		decltype(auto) value = *iter;
		if (p(value))
			return value;
	}
}

template<class Iter
	,bool FwdIter = is_forward_iterator_v<Iter>>
struct _filter_iter_storage
{
	_filter_iter_storage(Iter iter, Iter end) :
		iter(iter),
		end(end)
	{ }

	template<class T>
	_filter_iter_storage(Iter iter, Iter end, T&&) :
		_filter_iter_storage(iter, end)
	{ }

	decltype(auto)
	value() const
	{
		return *this->iter;
	}

	template<class T>
	void
	value(T&&)
	{ }

	Iter iter;
	Iter end;
};

template<class Iter>
struct _filter_iter_storage<Iter, false>
{
	_filter_iter_storage(Iter iter, Iter end) :
		iter(iter),
		end(end),
		val(std::nullopt)
	{ }

	_filter_iter_storage(Iter iter, Iter end, iter::value_type<Iter> value) :
		iter(iter),
		end(end),
		val(mtk::_move(value))
	{ }

	decltype(auto)
	value() const
	{
		MTK_ASSERT(this->val.has_value());
		return *this->val;
	}

	void
	value(iter::value_type<Iter> value)
	{
		this->val = mtk::_move(value);
	}

	Iter iter;
	Iter end;
	std::optional<iter::value_type<Iter>> val;
};

template<class Iter
	,class Predicate>
class _filter_iterator
{
public:
	using value_type = iter::value_type<Iter>;
	using reference = std::conditional_t<is_forward_iterator_v<Iter>, iter::reference<Iter>, value_type>;
	using pointer = struct {
		reference value;
		auto
		operator->() const
		{
			return mtk::_addressof(this->value);
		}
	};
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::input_iterator_tag;

	template<class P
		,_require<std::is_constructible_v<Predicate, P>> = 0>
	_filter_iterator(Iter iter, Iter end, P&& p) :
		m_storage(iter, end),
		m_pred(mtk::_forward<P>(p))
	{
		if (m_storage.iter == m_storage.end)
			return;

		decltype(auto) value = *m_storage.iter;
		if (!m_pred.get()(value))
			++*this;
		else
			m_storage.value(mtk::_move(value));
	}

	template<class T
		,_require<std::is_convertible_v<T, value_type>> = 0>
	_filter_iterator(Iter iter, Iter end, T&& t) :
		m_storage(iter, end, mtk::_forward(t)),
		m_pred()
	{ }

	_filter_iterator(Iter iter, Iter end) :
		m_storage(iter, end),
		m_pred()
	{ }

	reference
	operator*() const
	{
		MTK_ASSERT(m_storage.iter != m_storage.end);
		return m_storage.value();
	}

	pointer
	operator->() const
	{
		return pointer{**this};
	}

	friend
	_filter_iterator&
	operator++(_filter_iterator& rhs)
	{
		MTK_ASSERT(rhs.m_storage.iter != rhs.m_storage.end);
		rhs.m_storage.value(impl_filter::_increment(rhs.m_storage.iter, rhs.m_storage.end, rhs.m_pred.get()));
		return rhs;
	}

	friend
	_filter_iterator
	operator++(_filter_iterator& lhs, int)
	{
		_filter_iterator cp(lhs.m_storage.iter, lhs.m_storage.end, lhs.m_storage.value());
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const _filter_iterator& lhs, const _filter_iterator& rhs)
	{
		return (lhs.m_storage.iter == rhs.m_storage.iter);
	}

	friend
	bool
	operator!=(const _filter_iterator& lhs, const _filter_iterator& rhs)
	{
		return (lhs.m_storage.iter != rhs.m_storage.iter);
	}


private:
	_filter_iter_storage<Iter> m_storage;
	_functor_storage<Predicate> m_pred;
};

} // namespace impl_filter



template<class Cont
	,class Predicate
#ifndef MTK_DOXYGEN
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_r_v<bool, std::decay_t<Predicate>, typename std::decay_t<Cont>::reference>> = 0
#endif
>
auto
filter(Cont&& cont, Predicate&& p)
{
	using std::begin;
	using std::end;

	using iterator = decltype(begin(cont));
	using filter_iter = impl_filter::_filter_iterator<iterator, std::decay_t<Predicate>>;

	filter_iter first(begin(cont), end(cont), mtk::_forward<Predicate>(p));
	filter_iter last(end(cont), end(cont));
	return mtk::range(first, last);
}



namespace impl_filter {

template<class Predicate>
struct _filter_builder
{
	Predicate pred;
};

template<class Cont
	,class Pred
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_r_v<bool, std::decay_t<Pred>, typename std::decay_t<Cont>::reference>> = 0
>
auto
operator|(Cont&& cont, const _filter_builder<Pred>& pred)
{
	return mtk::filter(mtk::_forward<Cont>(cont), pred.pred);
}

template<class Cont
	,class Pred
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_r_v<bool, std::decay_t<Pred>, typename std::decay_t<Cont>::reference>> = 0
>
auto
operator|(Cont&& cont, _filter_builder<Pred>&& pred)
{
	return mtk::filter(mtk::_forward<Cont>(cont), mtk::_move(pred.pred));
}

} // namespace impl_filter



template<class Pred>
auto
filter(Pred&& p)
{
	return impl_filter::_filter_builder<std::decay_t<Pred>>{mtk::_forward<Pred>(p)};
}

} // namespace mtk

#endif
