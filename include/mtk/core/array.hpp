#ifndef MTK_CORE_ARRAY_HPP
#define MTK_CORE_ARRAY_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/unique_ptr.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <initializer_list>
#include <type_traits>

namespace mtk {
namespace impl_array {

template<class T
	,class = void>
struct _is_subtractable :
	std::false_type { };

template<class T>
struct _is_subtractable<T
		,void_t<decltype(mtk::_declval<T>() - mtk::_declval<T>())>
	> : std::true_type { };

template<class Iter>
constexpr
void
_copy_range(Iter first, Iter last, Iter dst)
{
	while (first != last) {
		*(dst++) = *(first++);
	}
}

template<class Iter>
constexpr
void
_move_range(Iter first, Iter last, Iter dst)
{
	while (first != last) {
		*(dst++) = mtk::_move(*(first++));
	}
}

template<class Iter>
constexpr
void
_swap_range(Iter first1, Iter last1, Iter first2)
{
	while (first1 != last1) {
		mtk::_swap(*(first1++), *(first2++));
	}
}

} // namespace impl_array

inline constexpr
size_t
dynamic_extent = static_cast<size_t>(-1);

template<class T
	,size_t N = dynamic_extent>
class array
{
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = pointer;
	using const_iterator = const_pointer;

	static constexpr size_type extent = N;



	std::conditional_t<(N > 0), value_type[N], char> _data;



	constexpr
	reference
	operator[](size_type pos)
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	constexpr
	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	constexpr
	reference
	front()
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	reference
	back()
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	constexpr
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	constexpr
	pointer
	data() noexcept
	{
		return reinterpret_cast<pointer>(&this->_data);
	}

	constexpr
	const_pointer
	data() const noexcept
	{
		return reinterpret_cast<const_pointer>(&this->_data);
	}



	constexpr
	iterator
	begin() noexcept
	{
		return this->data();
	}

	constexpr
	const_iterator
	begin() const noexcept
	{
		return this->data();
	}

	constexpr
	const_iterator
	cbegin() const noexcept
	{
		return this->begin();
	}

	constexpr
	iterator
	end() noexcept
	{
		return this->begin() + this->size();
	}

	constexpr
	const_iterator
	end() const noexcept
	{
		return this->begin() + this->size();
	}

	constexpr
	const_iterator
	cend() const noexcept
	{
		return this->end();
	}



	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	constexpr
	size_type
	size() const noexcept
	{
		return N;
	}

	constexpr
	size_type
	max_size() const noexcept
	{
		return this->size();
	}

	constexpr
	void
	swap(array& other)
	noexcept(std::is_nothrow_swappable_v<decltype(_data)>)
	{
		impl_array::_swap_range(this->begin(), this->end(), other.begin());
	}
};

template<class T>
class array<T, dynamic_extent>
{
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = pointer;
	using const_iterator = const_pointer;

	static constexpr size_type extent = dynamic_extent;



	array() noexcept :
		m_data(nullptr),
		m_size(0)
	{ }

	explicit
	array(size_type size) :
		m_data(size == 0 ? nullptr : mtk::make_unique<value_type[]>(size)),
		m_size(size)
	{ }

	array(std::initializer_list<value_type> ilist) :
		array(ilist.begin(), ilist.end())
	{ }

	template<class InputIter
#ifndef MTK_DOXYGEN
		,void_t<decltype(*mtk::_declval<InputIter>())>* = nullptr
		,void_t<decltype(*(mtk::_declval<InputIter&>()++))>* = nullptr
#endif
	>
	array(InputIter first, InputIter last) :
		array()
	{
		if constexpr (impl_array::_is_subtractable<InputIter>::value)
			this->_assign_ra_iter(first, last);
		else
			this->_assign_input_iter(first, last);
	}

	array(const array& other) :
		array(other.begin(), other.end())
	{ }

	array(array&& other) noexcept :
		m_data(mtk::_exchange(other.m_data, nullptr)),
		m_size(mtk::_exchange(other.m_size, 0))
	{ }

	array&
	operator=(array rhs) noexcept
	{
		this->swap(rhs);
		return *this;
	}



	reference
	operator[](size_type pos)
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	reference
	front()
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	reference
	back()
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	pointer
	data() noexcept
	{
		return m_data.get();
	}

	const_pointer
	data() const noexcept
	{
		return m_data.get();
	}



	iterator
	begin() noexcept
	{
		return this->data();
	}

	const_iterator
	begin() const noexcept
	{
		return this->data();
	}

	const_iterator
	cbegin() const noexcept
	{
		return this->begin();
	}

	iterator
	end() noexcept
	{
		return this->begin() + this->size();
	}

	const_iterator
	end() const noexcept
	{
		return this->begin() + this->size();
	}

	const_iterator
	cend() const noexcept
	{
		return this->end();
	}



	[[nodiscard]]
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	size_type
	size() const noexcept
	{
		return m_size;
	}

	size_type
	max_size() const noexcept
	{
		return static_cast<size_type>(-1) - 1;
	}

	void
	resize(size_type new_size)
	{
		if (new_size == this->size())
			return;

		array tmp(new_size);
		const size_type transfer_size = (new_size < this->size() ? new_size : this->size());
		if constexpr (std::is_nothrow_move_assignable_v<value_type>)
			impl_array::_move_range(this->begin(), this->begin() + transfer_size, tmp.begin());
		else
			impl_array::_copy_range(this->begin(), this->begin() + transfer_size, tmp.begin());

		this->swap(tmp);
	}



	void
	swap(array& other) noexcept
	{
		mtk::_swap(m_data, other.m_data);
		mtk::_swap(m_size, other.m_size);
	}

private:
	template<class InputIter>
	void
	_assign_input_iter(InputIter first, InputIter last)
	{
		array tmp(8);
		size_type count = 0;
		auto it = tmp.begin();
		while (first != last) {
			if (it == tmp.end()) {
				array tmp2(tmp.size()*2);
				impl_array::_move_range(tmp.begin(), tmp.end(), tmp2.begin());
				tmp.swap(tmp2);
				it = tmp.begin() + tmp2.size();
			}

			++count;
			*(it++) = *(first++);
		}

		array final_array(count);
		impl_array::_move_range(tmp.begin(), tmp.begin() + count, final_array.begin());
		this->swap(final_array);
	}

	template<class RAIter>
	void
	_assign_ra_iter(RAIter first, RAIter last)
	{
		MTK_ASSERT((last - first) >= 0);
		array tmp(last - first);
		auto it = tmp.begin();
		while (first != last) {
			*(it++) = *(first++);
		}
		this->swap(tmp);
	}

	unique_ptr<value_type[]> m_data;
	size_type m_size;
};

template<class T
	,class... Args
#ifndef MTK_DOXYGEN
	,_require<(std::is_same_v<T, Args> && ...)> = 0
#endif
>
array(T, Args...) -> array<T, 1 + sizeof...(Args)>;



template<class T
	,size_t N>
constexpr
void
swap(array<T, N>& a, array<T, N>& b)
noexcept(noexcept(a.swap(b)))
{
	a.swap(b);
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator==(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	for (auto it = lhs.begin(), e = lhs.end(), first = rhs.back(); it != e; ++it, ++first) {
		if (*it != *first)
			return false;
	}

	return true;
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator!=(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return !(lhs == rhs);
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator<(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	auto it = lhs.begin();
	const auto end = lhs.end();
	auto first = rhs.begin();
	const auto last = rhs.end();

	while ((it != end) && (first != last)) {
		if (*it < *first)
			return true;
		else if (*first < *it)
			return false;

		++it;
		++first;
	}

	return ((it == end) && (first != last));
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator>(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return (rhs < lhs);
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator<=(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return !(rhs < lhs);
}

template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator>=(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return !(lhs < rhs);
}

} // namespace mtk

#endif
