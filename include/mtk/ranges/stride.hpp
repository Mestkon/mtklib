#ifndef MTK_RANGES_STRIDE_HPP
#define MTK_RANGES_STRIDE_HPP

#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/types.hpp>
#include <mtk/ranges/range.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <iterator>
#include <type_traits>

namespace mtk {
namespace impl_stride {

template<class Iter>
class _stride_iterator
{
public:
	using value_type = iter::value_type<Iter>;
	using reference = iter::reference<Iter>;
	using pointer = iter::pointer<Iter>;
	using difference_type = iter::difference_type<Iter>;
	using iterator_category = std::input_iterator_tag;
	using _index_type = std::make_unsigned_t<difference_type>;

	constexpr
	_stride_iterator(Iter iter, Iter end, _index_type stride) :
		m_iter(iter),
		m_end(end),
		m_stride(stride)
	{ }

	constexpr
	reference
	operator*() const
	{
		MTK_ASSERT(m_iter != m_end);
		return *m_iter;
	}

	constexpr
	pointer
	operator->() const
	{
		if constexpr (std::is_class_v<Iter>)
			return m_iter.operator->();
		else
			return m_iter;
	}

	friend constexpr
	_stride_iterator&
	operator++(_stride_iterator& rhs)
	{
		if constexpr (is_random_access_iterator_v<Iter>) {
			const auto dst = static_cast<_index_type>(std::distance(rhs.m_iter, rhs.m_end));
			const auto to_move = (dst < rhs.m_stride ? dst : rhs.m_stride);
			rhs.m_iter += to_move;
		} else {
			_index_type count = 0;
			while ((rhs.m_iter != rhs.m_end) && (count < rhs.m_stride)) {
				++rhs.m_iter;
				++count;
			}
		}
		return rhs;
	}

	friend constexpr
	_stride_iterator
	operator++(_stride_iterator& lhs, int)
	{
		_stride_iterator cp = lhs;
		++lhs;
		return lhs;
	}

	friend constexpr
	bool
	operator==(const _stride_iterator& lhs, const _stride_iterator& rhs)
	{
		return (lhs.m_iter == rhs.m_iter);
	}

	friend constexpr
	bool
	operator!=(const _stride_iterator& lhs, const _stride_iterator& rhs)
	{
		return (lhs.m_iter != rhs.m_iter);
	}

private:
	Iter m_iter;
	Iter m_end;
	_index_type m_stride;
};

} // namespace impl_stride



template<class Cont>
constexpr
auto
stride(Cont&& cont, typename std::decay_t<Cont>::size_type to_stride)
{
	using std::begin;
	using std::end;
	impl_stride::_stride_iterator first(begin(cont), end(cont), to_stride);
	impl_stride::_stride_iterator last(end(cont), end(cont), to_stride);
	return mtk::range(first, last);
}



namespace impl_stride {

struct _stride_builder
{
	size_t stride;
};

template<class Cont
	,_require<is_input_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
>
constexpr
auto
operator|(Cont&& cont, _stride_builder stride)
{
	return mtk::stride(mtk::_forward<Cont>(cont), stride.stride);
}

} // namespace impl_stride



inline constexpr
auto
stride(size_t to_stride)
{
	return impl_stride::_stride_builder{to_stride};
}

} // namespace mtk

#endif
