#ifndef MTK_CORE_SPAN_HPP
#define MTK_CORE_SPAN_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T>
class span
{
public:
	using element_type = T;
	using value_type = std::remove_cv_t<T>;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using iterator = pointer;

	static constexpr size_t extent = static_cast<size_t>(-1);

	constexpr
	span() noexcept :
		span(nullptr, 0)
	{ }

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
	{ }

	template<size_t N>
	constexpr
	span(element_type(&arr)[N]) noexcept :
		span(arr, N)
	{ }

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

	constexpr
	iterator
	begin() const noexcept
	{
		return m_ptr;
	}

	constexpr
	iterator
	end() const noexcept
	{
		return m_ptr + m_size;
	}

	constexpr
	reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	constexpr
	reference
	operator[](size_type idx) const
	{
		MTK_ASSERT(idx < this->size());
		return *(this->begin() + idx);
	}

	constexpr
	pointer
	data() const noexcept
	{
		return this->begin();
	}

	constexpr
	size_type
	size() const noexcept
	{
		return m_size;
	}

	constexpr
	size_type
	size_bytes() const noexcept
	{
		return this->size()*sizeof(element_type);
	}

	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	constexpr
	span
	first(size_type count) const
	{
		MTK_ASSERT(count <= this->size());
		return span(this->data(), count);
	}

	constexpr
	span
	last(size_type count) const
	{
		MTK_ASSERT(count <= this->size());
		return span(this->data() + (this->size() - count), count);
	}

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

template<class U>
span(U*, size_t) -> span<U>;

template<class Range>
span(Range&) -> span<std::remove_pointer_t<decltype(mtk::_declval<Range&>().data())>>;

} // namespace mtk

#endif
