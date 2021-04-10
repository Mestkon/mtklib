#ifndef MTK_CORE_POINTER_HPP
#define MTK_CORE_POINTER_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/nullptr_exception.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/require.hpp>

#include <limits>
#include <type_traits>

namespace mtk {

template<class T>
class pointer
{
public:
	using pointer_type = T*;
	using element_type = T;

	constexpr
	pointer() noexcept :
		m_ptr(nullptr)
	{ }

	constexpr
	pointer(pointer_type ptr) noexcept :
		m_ptr(ptr)
	{ }

	constexpr
	pointer&
	operator=(pointer_type rhs) noexcept
	{
		m_ptr = rhs;
		return *this;
	}

	constexpr
	operator pointer_type() const noexcept
	{
		return m_ptr;
	}

	explicit constexpr
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	constexpr
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	constexpr
	element_type&
	operator*() const
	{
		if (!m_ptr)
			mtk::_throw_nullptr_exception();
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const
	{
		if (!m_ptr)
			mtk::_throw_nullptr_exception();
		return m_ptr;
	}

	template<class U>
	auto operator[](U&&) = delete;

	constexpr
	void
	reset(pointer_type ptr = nullptr) noexcept
	{
		m_ptr = ptr;
	}

	constexpr
	void
	swap(pointer& other) noexcept
	{
		auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}

private:
	pointer_type m_ptr;
};

template<class T> auto operator++(pointer<T>&) = delete;
template<class T> auto operator++(pointer<T>&, int) = delete;
template<class T> auto operator--(pointer<T>&) = delete;
template<class T> auto operator--(pointer<T>&, int) = delete;

template<class T> auto operator+=(pointer<T>&, ptrdiff_t) = delete;
template<class T> auto operator-=(pointer<T>&, ptrdiff_t) = delete;
template<class T> auto operator+(pointer<T>, ptrdiff_t) = delete;
template<class T> auto operator+(ptrdiff_t, pointer<T>) = delete;
template<class T> auto operator-(pointer<T>, ptrdiff_t) = delete;
template<class T> auto operator-(pointer<T>, pointer<T>) = delete;



template<class T>
class pointer<T[]>
{
public:
	using pointer_type = T*;
	using element_type = T;

	constexpr
	pointer() noexcept :
		m_ptr(nullptr)
	{ }

	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	constexpr
	pointer(U p) noexcept :
		m_ptr(p)
	{ }

	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	constexpr
	pointer&
	operator=(U rhs) noexcept
	{
		m_ptr = rhs;
		return *this;
	}

	constexpr
	operator pointer_type() noexcept
	{
		return m_ptr;
	}

	explicit constexpr
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	constexpr
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	constexpr
	element_type&
	operator*() const
	{
		if (!m_ptr)
			mtk::_throw_nullptr_exception();
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const
	{
		if (!m_ptr)
			mtk::_throw_nullptr_exception();
		return m_ptr;
	}

	constexpr
	element_type&
	operator[](size_t idx) const
	{
		MTK_ASSERT(idx <= static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max()));
		return *(*this + static_cast<ptrdiff_t>(idx));
	}

#ifndef MTK_DOXYGEN
	template<class Integer
		,_require<std::is_integral_v<Integer>> = 0>
	constexpr
	element_type&
	operator[](Integer idx) const
	{
		return (*this)[static_cast<size_t>(idx)];
	}
#endif

	template<class U = std::nullptr_t
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	constexpr
	void
	reset(U ptr = nullptr) noexcept
	{
		m_ptr = ptr;
	}

	constexpr
	void
	swap(pointer& other) noexcept
	{
		auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}



	friend constexpr
	pointer&
	operator++(pointer& rhs)
	{
		if (!rhs)
			mtk::_throw_nullptr_exception();
		++rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	pointer
	operator++(pointer& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	pointer&
	operator--(pointer& rhs)
	{
		if (!rhs)
			mtk::_throw_nullptr_exception();
		--rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	pointer
	operator--(pointer& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}



	friend constexpr
	pointer&
	operator+=(pointer& lhs, ptrdiff_t rhs)
	{
		if (!lhs)
			mtk::_throw_nullptr_exception();
		lhs.m_ptr += rhs;
		return lhs;
	}

	friend constexpr
	pointer
	operator+(pointer lhs, ptrdiff_t rhs)
	{
		lhs += rhs;
		return lhs;
	}

	friend constexpr
	pointer
	operator+(ptrdiff_t lhs, pointer rhs)
	{
		rhs += lhs;
		return rhs;
	}

	friend constexpr
	pointer&
	operator-=(pointer& lhs, ptrdiff_t rhs)
	{
		if (!lhs)
			mtk::_throw_nullptr_exception();
		lhs.m_ptr -= rhs;
		return lhs;
	}

	friend constexpr
	pointer
	operator-(pointer lhs, ptrdiff_t rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer lhs, pointer rhs)
	{
		if (!lhs || !rhs)
			mtk::_throw_nullptr_exception();
		return (lhs.m_ptr - rhs.m_ptr);
	}

private:
	pointer_type m_ptr;
};

template<class T>
constexpr
void
swap(pointer<T>& a, pointer<T>& b) noexcept
{
	a.swap(b);
}

} // namespace mtk

#endif
