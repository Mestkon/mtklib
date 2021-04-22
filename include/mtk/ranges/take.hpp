#ifndef MTK_RANGES_TAKE_HPP
#define MTK_RANGES_TAKE_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/iterator_traits.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {
namespace impl_take {

template<class Iter>
class _take_iterator
{
public:
	using value_type = iter::value_type<Iter>;
	using reference = iter::reference<Iter>;
	using pointer = iter::pointer<Iter>;
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::input_iterator_tag;
	using _index_type = std::make_unsigned_t<difference_type>;

	constexpr
	_take_iterator(Iter iter, _index_type take, _index_type idx) :
		m_iter(iter),
		m_take(take),
		m_idx(idx)
	{ }

	constexpr
	reference
	operator*() const
	{
		return *m_iter;
	}

	constexpr
	Iter
	operator->() const
	{
		return m_iter;
	}

	friend constexpr
	_take_iterator&
	operator++(_take_iterator& rhs)
	{
		MTK_ASSERT(rhs.m_idx < rhs.m_take);
		++rhs.m_iter;
		++rhs.m_idx;
		return rhs;
	}

	friend constexpr
	_take_iterator
	operator++(_take_iterator& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	bool
	operator==(const _take_iterator& lhs, const _take_iterator& rhs)
	{
		return (lhs.m_idx == rhs.m_idx);
	}

	friend constexpr
	bool
	operator!=(const _take_iterator& lhs, const _take_iterator& rhs)
	{
		return (lhs.m_idx != rhs.m_idx);
	}

private:
	Iter m_iter;
	_index_type m_take;
	_index_type m_idx;
};

} // namespace impl_take



template<class Cont
#ifndef MTK_DOXYGEN
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
#endif
>
constexpr
auto
take(Cont&& c, typename std::decay_t<Cont>::size_type to_take)
{
	using std::begin;
	impl_take::_take_iterator first(begin(c), to_take, 0);
	impl_take::_take_iterator last(begin(c), to_take, to_take);
	return mtk::range(first, last);
}

} // namespace mtk

#endif
