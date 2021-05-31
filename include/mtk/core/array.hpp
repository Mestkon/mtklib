#ifndef MTK_CORE_ARRAY_HPP
#define MTK_CORE_ARRAY_HPP

//! @file
//! Contains the definition of mtk::array.

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/unique_ptr.hpp>
#include <mtk/core/impl/algorithm.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/dynamic_extent.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <initializer_list>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

namespace impl_core {
namespace array {

template<class T
	,class = void>
struct _is_subtractable :
	std::false_type { };

template<class T>
struct _is_subtractable<T
		,_void_t<decltype(mtk::_declval<T>() - mtk::_declval<T>())>
	> : std::true_type { };



[[noreturn]]
void
_throw_out_of_range_exception(size_t idx, size_t size);


} // namespace array
} // namespace impl_core



//! @brief Lightweight replacement for std::array.
//!
//! @code
//! #include <mtk/core/array.hpp>
//! @endcode
//!
//! Lightweight as in includes less of the standard library.
//!
//! Difference between std::array and mtk::array:
//! - mtk::array does not support member function reverse iterators (e.g. arr.rbegin()),
//!   #include <mtk/core/reverse_iterators.hpp> for the free function reverse iterators (e.g. rbegin(arr)).
//!   The purpose of this is to avoid #include <iterator> if not necessary.
//! - mtk::array supports dynamic sized arrays by setting N = mtk::dynamic_extent.
//!
//! Template Specializations:
//!	- mtk::array<T, dynamic_extent>
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



	//! @brief Returns the element at position pos.
	//!
	//! @pre pos < this->size().
	constexpr
	reference
	operator[](size_type pos)
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	//! @brief Returns the element at position pos.
	//!
	//! @pre pos < this->size().
	constexpr
	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	//! Returns the element at position pos. Throws std::out_of_range() if pos >= this->size().
	constexpr
	reference
	at(size_type pos)
	{
		if (pos >= this->size())
			mtk::impl_core::array::_throw_out_of_range_exception(pos, this->size());

		return *(this->begin() + pos);
	}

	//! Returns the element at position pos. Throws std::out_of_range() if pos >= this->size().
	constexpr
	const_reference
	at(size_type pos) const
	{
		if (pos >= this->size())
			mtk::impl_core::array::_throw_out_of_range_exception(pos, this->size());

		return *(this->begin() + pos);
	}

	//! @brief Returns the first element.
	//!
	//! @pre this->empty() == false.
	constexpr
	reference
	front()
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns the first element.
	//!
	//! @pre this->empty() == false.
	constexpr
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns the last element.
	//!
	//! @pre this->empty() == false.
	constexpr
	reference
	back()
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! @brief Returns the last element.
	//!
	//! @pre this->empty() == false.
	constexpr
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! Returns a pointer to the first element in the array.
	constexpr
	pointer
	data() noexcept
	{
		return reinterpret_cast<pointer>(&this->_data);
	}

	//! Returns a pointer to the first element in the array.
	constexpr
	const_pointer
	data() const noexcept
	{
		return reinterpret_cast<const_pointer>(&this->_data);
	}



	//! Returns an iterator to the beginning of the array range.
	constexpr
	iterator
	begin() noexcept
	{
		return this->data();
	}

	//! Returns an iterator to the beginning of the array range.
	constexpr
	const_iterator
	begin() const noexcept
	{
		return this->data();
	}

	//! Returns an iterator to the beginning of the array range.
	constexpr
	const_iterator
	cbegin() const noexcept
	{
		return this->begin();
	}

	//! Returns an iterator to the end of the array range.
	constexpr
	iterator
	end() noexcept
	{
		return this->begin() + this->size();
	}

	//! Returns an iterator to the end of the array range.
	constexpr
	const_iterator
	end() const noexcept
	{
		return this->begin() + this->size();
	}

	//! Returns an iterator to the end of the array range.
	constexpr
	const_iterator
	cend() const noexcept
	{
		return this->end();
	}



	//! Returns this->size() == 0.
	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	//! Returns the number of elements in the array.
	constexpr
	size_type
	size() const noexcept
	{
		return N;
	}

	//! Returns the theoretic max number of elements possible to store in the array.
	constexpr
	size_type
	max_size() const noexcept
	{
		return this->size();
	}

	//! Swaps the contents of this array and other.
	constexpr
	void
	swap(array& other)
	noexcept(std::is_nothrow_swappable_v<decltype(_data)>)
	{
		mtk::_swap_range(this->begin(), this->end(), other.begin());
	}
};

//! @brief Specialization for dynamic_extent.
//!
//! @code
//! #include <mtk/core/array.hpp>
//! @endcode
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


	//! Constructs an empty array.
	array() noexcept :
		m_data(nullptr),
		m_size(0)
	{ }

	//! Constructs an array with size default constructed elements.
	explicit
	array(size_type size) :
		m_data(size == 0 ? nullptr : mtk::make_unique<value_type[]>(size)),
		m_size(size)
	{ }

	//! Constructs an array initialized as a copy of the provided range.
	array(std::initializer_list<value_type> ilist) :
		array(ilist.begin(), ilist.end())
	{ }

	//! Constructs an array initialized as a copy of the provided range.
	template<class InputIter
#ifndef MTK_DOXYGEN
		,_void_t<decltype(*mtk::_declval<InputIter>())>* = nullptr
		,_void_t<decltype(*(mtk::_declval<InputIter&>()++))>* = nullptr
#endif
	>
	array(InputIter first, InputIter last) :
		array()
	{
		if constexpr (impl_core::array::_is_subtractable<InputIter>::value)
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


	//! @brief Returns the element at position pos.
	//!
	//! @pre pos < this->size().
	reference
	operator[](size_type pos)
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	//! @brief Returns the element at position pos.
	//!
	//! @pre pos < this->size().
	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	//! Returns the element at position pos. Throws std::out_of_range() if pos >= this->size().
	reference
	at(size_type pos)
	{
		if (pos >= this->size())
			mtk::impl_core::array::_throw_out_of_range_exception(pos, this->size());

		return *(this->begin() + pos);
	}

	//! Returns the element at position pos. Throws std::out_of_range() if pos >= this->size().
	const_reference
	at(size_type pos) const
	{
		if (pos >= this->size())
			mtk::impl_core::array::_throw_out_of_range_exception(pos, this->size());

		return *(this->begin() + pos);
	}

	//! @brief Returns the first element.
	//!
	//! @pre this->empty() == false.
	reference
	front()
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns the first element.
	//!
	//! @pre this->empty() == false.
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns the last element.
	//!
	//! @pre this->empty() == false.
	reference
	back()
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! @brief Returns the last element.
	//!
	//! @pre this->empty() == false.
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! Returns a pointer to the first element in the array.
	pointer
	data() noexcept
	{
		return m_data.get();
	}

	//! Returns a pointer to the first element in the array.
	const_pointer
	data() const noexcept
	{
		return m_data.get();
	}



	//! Returns an iterator to the beginning of the array range.
	iterator
	begin() noexcept
	{
		return this->data();
	}

	//! Returns an iterator to the beginning of the array range.
	const_iterator
	begin() const noexcept
	{
		return this->data();
	}

	//! Returns an iterator to the beginning of the array range.
	const_iterator
	cbegin() const noexcept
	{
		return this->begin();
	}

	//! Returns an iterator to the end of the array range.
	iterator
	end() noexcept
	{
		return this->begin() + this->size();
	}

	//! Returns an iterator to the end of the array range.
	const_iterator
	end() const noexcept
	{
		return this->begin() + this->size();
	}

	//! Returns an iterator to the end of the array range.
	const_iterator
	cend() const noexcept
	{
		return this->end();
	}



	//! Returns this->size() == 0.
	[[nodiscard]]
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	//! Returns the number of elements in the array.
	size_type
	size() const noexcept
	{
		return m_size;
	}

	//! Returns the theoretic max number of elements possible to store in the array.
	size_type
	max_size() const noexcept
	{
		return static_cast<size_type>(-1) - 1;
	}

	//! @brief Resizes the current array to new_size.
	//!
	//! If new_size < this->size() then the excess elements are pruned,
	//! else if new_size > this->size() then the new values are default initialized at the end
	//! of the current array.
	//!
	//! Invalidates all iterators if new_size != this->size().
	void
	resize(size_type new_size)
	{
		if (new_size == this->size())
			return;

		array tmp(new_size);
		const size_type transfer_size = (new_size < this->size() ? new_size : this->size());
		if constexpr (std::is_nothrow_move_assignable_v<value_type>)
			mtk::_move_range(this->begin(), this->begin() + transfer_size, tmp.begin());
		else
			mtk::_copy_range(this->begin(), this->begin() + transfer_size, tmp.begin());

		this->swap(tmp);
	}



	//! Swaps the contents of this array and other.
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
				tmp.resize(tmp.size()*2);
				it = tmp.begin() + count;
			}

			++count;
			*(it++) = *(first++);
		}

		array final_array(count);
		mtk::_move_range(tmp.begin(), tmp.begin() + count, final_array.begin());
		this->swap(final_array);
	}

	template<class RAIter>
	void
	_assign_ra_iter(RAIter first, RAIter last)
	{
		MTK_ASSERT((last - first) >= 0);
		array tmp(last - first);
		mtk::_copy_range(first, last, tmp.begin());
		this->swap(tmp);
	}

	unique_ptr<value_type[], empty_pointer_validator> m_data;
	size_type m_size;
};

//! @}

//! @brief Deduction guide.
//!
//! @relates array
template<class T
	,class... Args
#ifndef MTK_DOXYGEN
	,_require<(std::is_same_v<T, Args> && ...)> = 0
#endif
>
array(T, Args...) -> array<T, 1 + sizeof...(Args)>;



//! @brief Swaps the contents of this array and other.
//!
//! @relates array
template<class T
	,size_t N>
constexpr
void
swap(array<T, N>& a, array<T, N>& b)
noexcept(noexcept(a.swap(b)))
{
	a.swap(b);
}

//! @brief Returns true if the contents of lhs and rhs are equal, else false.
//!
//! @relates array
template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator==(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	return mtk::_equal_range(lhs.begin(), lhs.end(), rhs.begin());
}

//! @brief Returns false if the contents of lhs and rhs are equal, else true.
//!
//! @relates array
template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator!=(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return !(lhs == rhs);
}

//! @brief Returns true if the elements of lhs is lexicographically less than rhs, else false.
//!
//! @relates array
template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator<(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return mtk::_less_than_range(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

//! @brief Returns true if the elements of lhs is lexicographically greater than rhs, else false.
//!
//! @relates array
template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator>(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return (rhs < lhs);
}

//! @brief Returns true if the elements of lhs is lexicographically less than or equal to rhs, else false.
//!
//! @relates array
template<class T
	,size_t N1
	,size_t N2>
constexpr
bool
operator<=(const array<T, N1>& lhs, const array<T, N2>& rhs)
{
	return !(rhs < lhs);
}

//! @brief Returns true if the elements of lhs is lexicographically greater than or equal to rhs, else false.
//!
//! @relates array
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
