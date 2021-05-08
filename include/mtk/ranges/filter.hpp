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
	,class Predicate>
class _filter_iterator
{
public:
	using value_type = iter::value_type<Iter>;
	using reference = iter::reference<Iter>;
	using pointer = iter::pointer<Iter>;
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::input_iterator_tag;

	template<class P
		,_require<std::is_constructible_v<Predicate, P>> = 0>
	_filter_iterator(Iter iter, Iter end, P&& p) :
		m_pred(mtk::_forward<P>(p)),
		m_iter(iter),
		m_end(end)
	{
		if (m_iter == m_end)
			return;

		if (!m_pred.get()(*m_iter))
			++*this;
	}

	_filter_iterator(Iter iter, Iter end) :
		m_pred(),
		m_iter(iter),
		m_end(end)
	{ }

	reference
	operator*() const
	{
		MTK_ASSERT(m_iter != m_end);
		return *m_iter;
	}

	pointer
	operator->() const
	{
		MTK_ASSERT(m_iter != m_end);
		if constexpr (std::is_class_v<Iter>)
			return m_iter.operator->();
		else
			return m_iter;
	}

	friend
	_filter_iterator&
	operator++(_filter_iterator& rhs)
	{
		MTK_ASSERT(rhs.m_iter != rhs.m_end);
		while (++rhs.m_iter != rhs.m_end) {
			if (rhs.m_pred.get()(*rhs.m_iter))
				break;
		}
		return rhs;
	}

	friend
	_filter_iterator
	operator++(_filter_iterator& lhs, int)
	{
		_filter_iterator cp(lhs.m_iter, lhs.m_end);
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const _filter_iterator& lhs, const _filter_iterator& rhs)
	{
		return (lhs.m_iter == rhs.m_iter);
	}

	friend
	bool
	operator!=(const _filter_iterator& lhs, const _filter_iterator& rhs)
	{
		return (lhs.m_iter != rhs.m_iter);
	}


private:
	_functor_storage<Predicate> m_pred;
	Iter m_iter;
	Iter m_end;
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
