#ifndef MTK_CORE_SPAN_HPP
#define MTK_CORE_SPAN_HPP

//! @file
//! Contains mtk::span

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Represents an array as pointer and size.
//!
//! @code
//! #include <mtk/core/span.h>
//! @endcode
template<class T>
class span
{
public:
	//! Typedef.
	using element_type = T;
	//! Typedef.
	using value_type = std::remove_cv_t<T>;
	//! Typedef.
	using size_type = size_t;
	//! Typedef.
	using difference_type = ptrdiff_t;
	//! Typedef.
	using pointer = T*;
	//! Typedef.
	using const_pointer = const T*;
	//! Typedef.
	using reference = T&;
	//! Typedef.
	using const_reference = const T&;
	//! Typedef.
	using iterator = pointer;

	//! Class variable to mach std::span.
	static constexpr size_t extent = static_cast<size_t>(-1);

	//! Constructs an empty span.
	constexpr
	span() noexcept :
		span(nullptr, 0)
	{ }

	//! @brief Constructs a span from pointer and size.
	//!
	//! @pre U must be convertible to element_type.
	//! @pre std::remove_cv_t<U> must be same as value_type.
	//! @pre If count > 0 then data != nullptr.
	//! @pre count must be <= the number of elements accessible from data.
	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<U, element_type>> = 0
		,_require<std::is_same_v<std::remove_cv_t<U>, value_type>> = 0
#endif
	>
	constexpr
	span(U* data, size_type count) :
		m_ptr(data),
		m_size(count)
	{
		MTK_ASSERT((count == 0) || bool(data));
	}

	//! Constructs a span from a c-style array.
	template<size_t N>
	constexpr
	span(element_type(&arr)[N]) noexcept :
		span(arr, N)
	{ }

	//! @brief Constructs a span from a range/container.
	//!
	//! @pre span(r.data(), r.size()) must be valid.
	template<class Range
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<std::remove_pointer_t<decltype(mtk::_declval<Range&>().data())>, element_type>> = 0
		,_require<std::is_same_v<std::remove_cv_t<std::remove_pointer_t<decltype(mtk::_declval<Range&>().data())>>, value_type>> = 0
		,_require<std::is_convertible_v<decltype(mtk::_declval<Range&>().size()), size_type>> = 0
#endif
	>
	constexpr
	span(Range& r) :
		span(r.data(), r.size())
	{ }

	//! @brief Constructs a span from a span with different type.
	//!
	//! @pre span(other.data(), other.size()) must be valid.
	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<U, element_type>> = 0
		,_require<std::is_same_v<std::remove_cv_t<U>, value_type>> = 0
#endif
	>
	constexpr
	span(const span<U>& other) noexcept :
		span(other.data(), other.size())
	{ }

	//! Returns iterator to begin.
	constexpr
	iterator
	begin() const noexcept
	{
		return this->data();
	}

	//! Returns iterator to end.
	constexpr
	iterator
	end() const noexcept
	{
		return this->data() + this->size();
	}

	//! @brief Returns a reference to the first element.
	//!
	//! @pre !empty()
	constexpr
	reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns a reference to the last element.
	//!
	//! @pre !empty()
	constexpr
	reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! @brief Returns a reference to the idx'th element.
	//!
	//! @pre idx < size().
	constexpr
	reference
	operator[](size_type idx) const
	{
		MTK_ASSERT(idx < this->size());
		return *(this->begin() + idx);
	}

	//! Returns a pointer to the array.
	constexpr
	pointer
	data() const noexcept
	{
		return m_ptr;
	}

	//! Returns the number of elements in the array.
	constexpr
	size_type
	size() const noexcept
	{
		return m_size;
	}

	//! Returns the number of bytes in the array.
	constexpr
	size_type
	size_bytes() const noexcept
	{
		return this->size()*sizeof(element_type);
	}

	//! Returns size() == 0.
	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	//! @brief Returns the first count elements in the array as a span.
	//!
	//! @pre count <= size().
	constexpr
	span
	first(size_type count) const
	{
		MTK_ASSERT(count <= this->size());
		return span(this->data(), count);
	}

	//! @brief Returns the last count elements in the array as a span.
	//!
	//! @pre count <= size().
	constexpr
	span
	last(size_type count) const
	{
		MTK_ASSERT(count <= this->size());
		return span(this->data() + (this->size() - count), count);
	}

	//! @brief Returns count elements beginning at offset as a span.
	//!
	//! Retruns all elements after offset if count == extent.
	//!
	//! @pre offset <= size().
	//! @pre count == extent || count <= (size - offset).
	constexpr
	span
	subspan(size_type offset, size_type count = extent) const
	{
		MTK_ASSERT(offset <= this->size());
		MTK_ASSERT((count == extent) || (count <= (this->size() - offset)));
		return span(this->data() + offset, (count == extent) ? this->size() - offset : count);
	}

private:
	pointer m_ptr;
	size_type m_size;
};

//! Deduction guide.
template<class U>
span(U*, size_t) -> span<U>;

//! Deduction guide.
template<class Range>
span(Range&) -> span<std::remove_pointer_t<decltype(mtk::_declval<Range&>().data())>>;

//! @}

} // namespace mtk

#endif
