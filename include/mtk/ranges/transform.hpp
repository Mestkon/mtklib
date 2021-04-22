#ifndef MTK_RANGES_TRANSFORM_HPP
#define MTK_RANGES_TRANSFORM_HPP

#include <mtk/core/iterator_traits.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/addressof.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/ranges/impl/predicate_storage.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {
namespace impl_transform {

template<class Iter
	,class Predicate>
class _transform_iterator
{
public:
	using value_type = decltype(mtk::_declval<Predicate&>()(*mtk::_declval<Iter>()));
	using reference = value_type;
	using pointer = struct {
		value_type value;
		auto
		operator->() const { return mtk::_addressof(this->value); }
	};
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::input_iterator_tag;

	template<class P>
	_transform_iterator(Iter iter, P&& p) :
		m_iter(iter),
		m_pred(mtk::_forward<P>(p))
	{ }

	_transform_iterator(Iter iter) :
		m_iter(iter),
		m_pred()
	{ }

	reference
	operator*() const
	{
		return m_pred.get()(*m_iter);
	}

	pointer
	operator->() const
	{
		return pointer{**this};
	}

	friend
	_transform_iterator&
	operator++(_transform_iterator& rhs)
	{
		++rhs.m_iter;
		return rhs;
	}

	friend
	_transform_iterator
	operator++(_transform_iterator& lhs, int)
	{
		_transform_iterator cp(lhs.m_iter);
		++lhs;
		return cp;
	}

	friend
	bool
	operator==(const _transform_iterator& lhs, const _transform_iterator& rhs)
	{
		return (lhs.m_iter == rhs.m_iter);
	}

	friend
	bool
	operator!=(const _transform_iterator& lhs, const _transform_iterator& rhs)
	{
		return (lhs.m_iter != rhs.m_iter);
	}

private:
	Iter m_iter;
	_predicate_storage<Predicate> m_pred;
};

} // namespace impl_transform



template<class Cont
	,class Predicate
#ifndef MTK_DOXYGEN
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_v<std::decay_t<Predicate>, typename std::decay_t<Cont>::reference>> = 0
#endif
>
auto
transform(Cont&& cont, Predicate&& p)
{
	using iterator = typename std::decay_t<Cont>::const_iterator;
	using transfom_iter = impl_transform::_transform_iterator<iterator, std::decay_t<Predicate>>;

	using std::begin;
	using std::end;
	transfom_iter first(begin(cont), mtk::_forward<Predicate>(p));
	transfom_iter last(end(cont));
	return mtk::range(first, last);
}



namespace impl_transform {

template<class Predicate>
struct _transfom_builder
{
	Predicate pred;
};

template<class Cont
	,class Pred
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_r_v<bool, std::decay_t<Pred>, typename std::decay_t<Cont>::reference>> = 0
>
auto
operator|(Cont&& cont, const _transfom_builder<Pred>& pred)
{
	return mtk::transform(mtk::_forward<Cont>(cont), pred.pred);
}

template<class Cont
	,class Pred
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
	,_require<std::is_invocable_r_v<bool, std::decay_t<Pred>, typename std::decay_t<Cont>::reference>> = 0
>
auto
operator|(Cont&& cont, _transfom_builder<Pred>&& pred)
{
	return mtk::transform(mtk::_forward<Cont>(cont), mtk::_move(pred.pred));
}

} // namespace impl_transform



template<class Pred>
auto
transform(Pred&& p)
{
	return impl_transform::_transfom_builder<std::decay_t<Pred>>{mtk::_forward<Pred>(p)};
}

} // namespace mtk

#endif
