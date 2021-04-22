#ifndef MTK_RANGES_RANGE_HPP
#define MTK_RANGES_RANGE_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/iterator_traits.hpp>
#include <mtk/core/reverse_iterators.hpp>
#include <mtk/core/impl/require.hpp>

#include <limits>
#include <iterator>
#include <type_traits>

namespace mtk {
namespace impl_range {

template<class Iter>
class _input_iterator_range_adapter
{
public:
	using value_type = iter::value_type<Iter>;
	using difference_type = iter::difference_type<Iter>;
	using size_type = std::make_unsigned_t<difference_type>;
	using pointer = iter::pointer<Iter>;
	using const_pointer = pointer;
	using reference = iter::reference<Iter>;
	using const_reference = reference;
	using iterator = Iter;
	using const_iterator = iterator;

	constexpr
	_input_iterator_range_adapter(iterator first, iterator last) :
		m_first(first),
		m_last(last)
	{ }

	constexpr
	iterator
	begin() const
	{
		return m_first;
	}

	constexpr
	iterator
	end() const
	{
		return m_last;
	}

	[[nodiscard]]
	constexpr
	bool
	empty() const
	{
		return (m_first == m_last);
	}

private:
	iterator m_first;
	iterator m_last;
};

template<class Iter>
class _bidirectional_iterator_range_adapter :
	public _input_iterator_range_adapter<Iter>
{
public:
	using _base = _input_iterator_range_adapter<Iter>;
	using typename _base::iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = reverse_iterator;

	using _base::_base;

	constexpr
	reverse_iterator
	rbegin() const
	{
		return reverse_iterator(this->end());
	}

	constexpr
	reverse_iterator
	rend() const
	{
		return reverse_iterator(this->begin());
	}
};

template<class Iter>
class _random_access_iterator_range_adapter :
	public _bidirectional_iterator_range_adapter<Iter>
{
public:
	using _base = _bidirectional_iterator_range_adapter<Iter>;
	using typename _base::size_type;
	using typename _base::reference;

	using _base::_base;

	constexpr
	reference
	operator[](size_type idx) const
	{
		MTK_ASSERT(idx < this->size());
		return *(this->begin() + idx);
	}

	constexpr
	size_type
	size() const
	{
		return static_cast<size_type>(this->end() - this->begin());
	}
};



template<class T
	,class StepT>
class _range_iterator
{
public:
	using value_type = T;
	using step_type = StepT;
	using pointer = struct {
		value_type value;
		constexpr
		const value_type*
		operator->() const { return &this->value; }
	};
	using reference = value_type;
	using difference_type = ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	constexpr
	_range_iterator(value_type val, step_type step) noexcept :
		m_val(val),
		m_step(step)
	{ }

	constexpr
	reference
	operator*() const noexcept
	{
		return m_val;
	}

	constexpr
	pointer
	operator->() const noexcept
	{
		return pointer{m_val};
	}

	friend constexpr
	_range_iterator&
	operator++(_range_iterator& rhs) noexcept
	{
		rhs.m_val += rhs.m_step;
		return rhs;
	}

	friend constexpr
	_range_iterator
	operator++(_range_iterator& lhs, int) noexcept
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	bool
	operator==(_range_iterator lhs, _range_iterator rhs) noexcept
	{
		return (lhs.m_val == rhs.m_val);
	}

	friend constexpr
	bool
	operator!=(_range_iterator lhs, _range_iterator rhs) noexcept
	{
		return (lhs.m_val != rhs.m_val);
	}

private:
	value_type m_val;
	step_type m_step;
};

} // namespace impl_range



template<class Iter
#ifndef MTK_DOXYGEN
	,_require<is_input_iterator_v<Iter>> = 0
#endif
>
constexpr
auto
range(Iter first, Iter last)
{
	if constexpr (is_random_access_iterator_v<Iter>)
		return impl_range::_random_access_iterator_range_adapter<Iter>(first, last);
	else if constexpr (is_bidirectional_iterator_v<Iter>)
		return impl_range::_bidirectional_iterator_range_adapter<Iter>(first, last);
	else
		return impl_range::_input_iterator_range_adapter<Iter>(first, last);
}

template<class Cont
#ifndef MTK_DOXYGEN
	,_require<is_bidirectional_iterator_v<typename std::decay_t<Cont>::iterator>> = 0
#endif
>
constexpr
auto
reversed(Cont&& c)
{
	using std::rbegin;
	using std::rend;
	return range(rbegin(c), rend(c));
}



template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_integral_v<T>> = 0
#endif
>
constexpr
auto
range(T end) noexcept
{
	if constexpr (std::is_signed_v<T>)
		MTK_ASSERT(end >= T());

	impl_range::_range_iterator first(T(), T(1));
	impl_range::_range_iterator last(end, T(1));
	return mtk::range(first, last);
}

template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_integral_v<T>> = 0
#endif
>
constexpr
auto
range(T begin, T end) noexcept
{
	MTK_ASSERT(end >= begin);

	impl_range::_range_iterator first(begin, T(1));
	impl_range::_range_iterator last(end, T(1));
	return mtk::range(first, last);
}

template<class T
	,class StepT
#ifndef MTK_DOXYGEN
	,_require<std::is_integral_v<T>> = 0
	,_require<std::is_integral_v<T>> = 0
#endif
>
constexpr
auto
range(T begin, T end, StepT step) noexcept
{
	MTK_ASSERT(step != StepT());
	MTK_ASSERT(step > StepT() ? end >= begin : end <= begin);

	const auto delta = (end >= begin ? end - begin : begin - end);
	const auto positive_step = (step < StepT() ? -step : step);
	const auto rem = (delta % positive_step);
	if (rem != T())
		end += (step > StepT() ? step - rem : step + rem);

	impl_range::_range_iterator first(begin, step);
	impl_range::_range_iterator last(end, step);
	return mtk::range(first, last);
}



inline constexpr
auto
integers = mtk::range(std::numeric_limits<ullong>::max());

} // namespace mtk

#endif
