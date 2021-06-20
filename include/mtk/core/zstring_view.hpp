#ifndef MTK_CORE_ZSTRING_VIEW_HPP
#define MTK_CORE_ZSTRING_VIEW_HPP

//! @file
//! Contains mtk::zstring_view

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <cstring>
#include <iosfwd>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

#ifdef MTK_DEBUG
namespace impl_core {
namespace zstring_view {

constexpr
size_t
_constexpr_strlen(const char* s) noexcept
{
	const char* e = s;
	while (*(e++) != '\0');
	return static_cast<size_t>(e - s - 1);
}

} // namespace zstring_view
} // namespace impl_core
#endif

//! @brief null-terminated version std::string_view
//!
//! @code
//! #include <mtk/core/zstring_view.hpp>
//! @endcode
//!
//! zstring_view is guaranteed to be either empty, or
//! null-terminated. If data() is not nullptr, then
//! data()[size()] is guaranteed to be 0.
class zstring_view
{
public:
	//! typedef.
	using value_type = char;
	//! typedef.
	using pointer = char*;
	//! typedef.
	using const_pointer = const char*;
	//! typedef.
	using reference = char&;
	//! typedef.
	using const_reference = const char&;
	//! typedef.
	using iterator = const_pointer;
	//! typedef.
	using const_iterator = const_pointer;
	//! typedef.
	using size_type = size_t;
	//! typedef.
	using difference_type = ptrdiff_t;

	//! Constructs an empy zstring_view.
	constexpr
	zstring_view() noexcept :
		m_ptr(nullptr),
		m_size(0)
	{ }

	//! @brief Constructs a zstring_view from ptr with size count.
	//!
	//! @pre ptr != nullptr.
	//! @pre ptr[count] == 0.
	constexpr
	zstring_view(const_pointer ptr, size_type count) noexcept :
		m_ptr(ptr),
		m_size(count)
	{
		MTK_ASSERT(ptr != nullptr);
		MTK_ASSERT(count == mtk::impl_core::zstring_view::_constexpr_strlen(ptr));
	}

	//! @brief Constructs a zstring_view from ptr.
	//!
	//! @pre ptr != nullptr.
	//! @pre ptr must be null-terminated.
#ifndef MTK_DOXYGEN
	template<class T
		,_require<std::is_pointer_v<T>> = 0
		,_require<std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char>> = 0>
	zstring_view(T ptr) :
#else
	zstring_view(const_pointer ptr) :
#endif
		m_ptr(ptr),
		m_size(0)
	{
		MTK_ASSERT(ptr != nullptr);
		m_size = std::strlen(ptr);
	}

	//! @brief Constructs a zstring_view from str.
	//!
	//! @pre str[N - 1] == 0.
	template<size_t N>
	constexpr
	zstring_view(const value_type (&str)[N]) noexcept :
		m_ptr(str),
		m_size(N - 1)
	{
		MTK_ASSERT(str[N - 1] == '\0');
	}

	//! Constructs a zstring_view from a std::string.
#ifndef MTK_DOXYGEN
	template<class StdString
		//check for random std::string specific interface
		,_void_t<typename StdString::traits_type>* = nullptr
		,_void_t<typename StdString::allocator_type>* = nullptr
		,_require<std::is_constructible_v<StdString, const char*>> = 0
		,_require<std::is_same_v<decltype(mtk::_declval<StdString&>().c_str()), const char*>> = 0
		,_void_t<decltype(mtk::_declval<StdString&>().length())>* = nullptr
		,_void_t<decltype(mtk::_declval<StdString&>().replace(size_t(), size_t(), mtk::_declval<StdString&>()))>* = nullptr
		,_void_t<decltype(mtk::_declval<StdString&>().find_first_not_of(mtk::_declval<StdString&>()))>* = nullptr
		,_void_t<decltype(StdString::npos)>* = nullptr
		,_void_t<decltype(mtk::_declval<StdString&>() += mtk::_declval<StdString&>())>* = nullptr
		,_void_t<decltype(mtk::_declval<StdString&>() += "")>* = nullptr
	>
	zstring_view(const StdString& s) noexcept :
#else
	zstring_view(const std::string& s) noexcept :
#endif
		m_ptr(s.c_str()),
		m_size(s.size())
	{ }

	//! Implicit conversion to std::string_view.
#ifndef MTK_DOXYGEN
	template<class StdStringView
		,_void_t<decltype(mtk::_declval<StdStringView&>().remove_prefix(size_t()))>* = nullptr
		,_void_t<decltype(mtk::_declval<StdStringView&>().remove_suffix(size_t()))>* = nullptr
		,_void_t<decltype(mtk::_declval<StdStringView&>().substr(size_t()))>* = nullptr
		,_void_t<decltype(mtk::_declval<StdStringView&>().find_first_not_of(mtk::_declval<StdStringView&>()))>* = nullptr
	>
	operator StdStringView() const noexcept
#else
	operator std::string_view() const noexcept
#endif
	{
		if (!this->empty())
			return StdStringView(this->data(), this->size() + 1);
		else
			return StdStringView();
	}



	//! Returns iterator to beginning.
	constexpr
	const_iterator
	begin() const noexcept
	{
		return this->data();
	}

	//! Returns iterator to beginning.
	constexpr
	const_iterator
	cbegin() const noexcept
	{
		return this->begin();
	}

	//! Returns iterator to end.
	constexpr
	const_iterator
	end() const noexcept
	{
		return this->data() + this->size();
	}

	//! Returns iterator to end.
	constexpr
	const_iterator
	cend() const noexcept
	{
		return this->end();
	}

	//! @brief Returns *(begin() + pos).
	//!
	//! @pre pos < size().
	constexpr
	const_reference
	operator[](size_type pos) const
	{
		MTK_ASSERT(pos < this->size());
		return *(this->begin() + pos);
	}

	//! @brief Returns the first character.
	//!
	//! @pre !empty().
	constexpr
	const_reference
	front() const
	{
		MTK_ASSERT(!this->empty());
		return *this->begin();
	}

	//! @brief Returns the last non-null character.
	//!
	//! @pre !empty().
	constexpr
	const_reference
	back() const
	{
		MTK_ASSERT(!this->empty());
		return *(this->end() - 1);
	}

	//! Returns a pointer to the char array.
	constexpr
	const_pointer
	data() const noexcept
	{
		return m_ptr;
	}

	//! @brief Returns the number of non-null characters in the array.
	//!
	//! If size() > 0 then data()[size()] is guaranteed to be 0.
	constexpr
	size_type
	size() const noexcept
	{
		return m_size;
	}

	//! @brief Returns the number of non-null characters in the array.
	//!
	//! If length() > 0 then data()[length()] is guaranteed to be 0.
	constexpr
	size_type
	length() const noexcept
	{
		return m_size;
	}

	//! Returns the theoretical maximum size of the stored string.
	constexpr
	size_type
	max_size() const noexcept
	{
		return static_cast<size_type>(-1) - 1;
	}

	//! Returns size() == 0.
	[[nodiscard]]
	constexpr
	bool
	empty() const noexcept
	{
		return (this->size() == 0);
	}

	//! Swaps the content of the zstring_views.
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

//! @}



//! @brief Retruns true if the contained c-strings are equivalent else false.
//!
//! @relates zstring_view
bool
operator==(zstring_view lhs, zstring_view rhs) noexcept;

//! @brief Retruns false if the contained c-strings are equivalent else true.
//!
//! @relates zstring_view
inline
bool
operator!=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(lhs == rhs);
}

//! @brief Retruns true if the contained c-string in lhs are
//! lexicographically less than the one in rhs else false.
//!
//! @relates zstring_view
bool
operator<(zstring_view lhs, zstring_view rhs) noexcept;

//! @brief Retruns true if the contained c-string in lhs are
//! lexicographically greater than the one in rhs else false.
//!
//! @relates zstring_view
inline
bool
operator>(zstring_view lhs, zstring_view rhs) noexcept
{
	return (rhs < lhs);
}

//! @brief Retruns true if the contained c-string in lhs are
//! lexicographically less than or equal to the one in rhs else false.
//!
//! @relates zstring_view
inline
bool
operator<=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(rhs < lhs);
}

//! @brief Retruns true if the contained c-string in lhs are
//! lexicographically greater than or equal to the one in rhs else false.
//!
//! @relates zstring_view
inline
bool
operator>=(zstring_view lhs, zstring_view rhs) noexcept
{
	return !(lhs < rhs);
}

//! @brief Outputs the string to an std::ostream object.
//!
//! @relates zstring_view
std::ostream&
operator<<(std::ostream& os, zstring_view zv);

//! @brief Swaps the content of a and b.
//!
//! @relates zstring_view
inline
void
swap(zstring_view& a, zstring_view& b) noexcept
{
	a.swap(b);
}

inline namespace literals {

//! Inline. Contains literals for zstring_view.
inline namespace zstring_view_literals {

//! @addtogroup core
//! @{

//! Returns the string literal as a zstring_view.
constexpr
zstring_view
operator""_zv(const char* str, size_t len) noexcept
{
	return zstring_view(str, len);
}

//! @}

} // namespace zstring_view_literals
} // namespace literals

} // namespace mtk

#endif
