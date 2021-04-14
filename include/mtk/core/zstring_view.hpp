#ifndef MTK_CORE_ZSTRING_VIEW_HPP
#define MTK_CORE_ZSTRING_VIEW_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <cstring>
#include <iosfwd>
#include <type_traits>

namespace mtk {

#ifdef MTK_DEBUG
constexpr
size_t
_constexpr_strlen(const char* s) noexcept
{
	const char* e = s;
	while (*(e++) != '\0');
	return static_cast<size_t>(e - s - 1);
}
#endif

class zstring_view
{
public:
	using value_type = char;
	using pointer = char*;
	using const_pointer = const char*;
	using reference = char&;
	using const_reference = const char&;
	using iterator = const_pointer;
	using const_iterator = const_pointer;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	constexpr
	zstring_view() noexcept :
		m_ptr(nullptr),
		m_size(0)
	{ }

	constexpr
	zstring_view(const char* ptr, size_type count) :
		m_ptr(ptr),
		m_size(count)
	{
		MTK_ASSERT(ptr != nullptr);
		MTK_ASSERT(count == _constexpr_strlen(ptr));
	}

	zstring_view(const char* ptr) :
		m_ptr(ptr),
		m_size(0)
	{
		MTK_ASSERT(ptr != nullptr);
		m_size = std::strlen(ptr);
	}

	template<class StdString
#ifndef MTK_DOXYGEN
		//check for random std::string specific interface
		,typename StdString::traits_type* = nullptr
		,typename StdString::allocator_type* = nullptr
		,_require<std::is_constructible_v<StdString, const char*>> = 0
		,_require<std::is_same_v<decltype(mtk::_declval<StdString&>().c_str()), const char*>> = 0
		,decltype(mtk::_declval<StdString&>().length())* = nullptr
		,std::remove_reference_t<decltype(mtk::_declval<StdString&>().replace(size_t(), size_t(), mtk::_declval<StdString&>()))>* = nullptr
		,decltype(mtk::_declval<StdString&>().find_first_not_of(mtk::_declval<StdString&>()))* = nullptr
		,decltype(StdString::npos)* = nullptr
		,std::remove_reference_t<decltype(mtk::_declval<StdString&>() += mtk::_declval<StdString&>())>* = nullptr
		,std::remove_reference_t<decltype(mtk::_declval<StdString&>() += "")>* = nullptr
#endif
	>
	zstring_view(const StdString& s) :
		m_ptr(s.c_str()),
		m_size(s.size())
	{ }

	template<class StdStringView
#ifndef MTK_DOXYGEN
		,decltype(mtk::_declval<StdStringView&>().remove_prefix(size_t()))* = nullptr
		,decltype(mtk::_declval<StdStringView&>().remove_suffix(size_t()))* = nullptr
		,decltype(mtk::_declval<StdStringView&>().substr(size_t()))* = nullptr
		,decltype(mtk::_declval<StdStringView&>().find_first_not_of(mtk::_declval<StdStringView&>()))* = nullptr
#endif
	>
	operator StdStringView() const noexcept
	{
		return StdStringView(this->data(), this->size() + 1);
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
	const_iterator
	end() const noexcept
	{
		return this->data() + this->size();
	}

	constexpr
	const_iterator
	cend() const noexcept
	{
		return this->end();
	}

	constexpr
	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	constexpr
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	constexpr
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	constexpr
	const_pointer
	data() const noexcept
	{
		return m_ptr;
	}

	constexpr
	size_type
	size() const noexcept
	{
		return m_size;
	}

	constexpr
	size_type
	length() const noexcept
	{
		return m_size;
	}

	constexpr
	size_type
	max_size() const noexcept
	{
		return static_cast<size_type>(-1) - 1;
	}

	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	constexpr
	void
	swap(zstring_view& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
		mtk::_swap(m_size, other.m_size);
	}

private:
	const_pointer m_ptr;
	size_type m_size;
};

bool
operator==(zstring_view lhs, zstring_view rhs) noexcept;

inline
bool
operator!=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(lhs == rhs);
}

bool
operator<(zstring_view lhs, zstring_view rhs) noexcept;

inline
bool
operator>(zstring_view lhs, zstring_view rhs) noexcept
{
	return (rhs < lhs);
}

inline
bool
operator<=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(rhs < lhs);
}

inline
bool
operator>=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(lhs < rhs);
}

std::ostream&
operator<<(std::ostream& os, zstring_view zv);

inline
void
swap(zstring_view& a, zstring_view& b) noexcept
{
	a.swap(b);
}

inline namespace literals {
inline namespace zstring_view_literals {

constexpr
zstring_view
operator""_zv(const char* str, size_t len) noexcept
{
	return zstring_view(str, len);
}

} // namespace zstring_view_literals
} // namespace literals

} // namespace mtk

#endif
