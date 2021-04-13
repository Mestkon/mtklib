#ifndef MTK_CORE_NOT_NULL_HPP
#define MTK_CORE_NOT_NULL_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/pointer_validator.hpp>
#include <mtk/core/impl/require.hpp>

#include <limits>
#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class not_null
{
public:
	using pointer_type = T*;
	using element_type = T;
	using validator_type = Validator;

	constexpr
	not_null(pointer_type ptr) :
		m_ptr(ptr)
	{
		validator_type()(m_ptr);
	}

	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get()), pointer_type>> = 0
#endif
	>
	constexpr
	not_null(const SmartPtr& ptr) :
		m_ptr(ptr.get())
	{
		validator_type()(m_ptr);
	}

	constexpr
	not_null&
	operator=(pointer_type rhs)
	{
		validator_type()(rhs);
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
	operator*() const noexcept
	{
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const noexcept
	{
		return m_ptr;
	}

	template<class U>
	auto operator[](U&&) = delete;

	constexpr
	void
	reset(pointer_type ptr)
	{
		validator_type()(ptr);
		m_ptr = ptr;
	}

	constexpr
	void
	swap(not_null& other) noexcept
	{
		const auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}

private:
	pointer_type m_ptr;
};

template<class T, class V> auto operator++(not_null<T, V>&) = delete;
template<class T, class V> auto operator++(not_null<T, V>&, int) = delete;
template<class T, class V> auto operator--(not_null<T, V>&) = delete;
template<class T, class V> auto operator--(not_null<T, V>&, int) = delete;

template<class T, class V> auto operator+=(not_null<T, V>&, ptrdiff_t) = delete;
template<class T, class V> auto operator-=(not_null<T, V>&, ptrdiff_t) = delete;
template<class T, class V> auto operator+(not_null<T, V>, ptrdiff_t) = delete;
template<class T, class V> auto operator+(ptrdiff_t, not_null<T, V>) = delete;
template<class T, class V> auto operator-(not_null<T, V>, ptrdiff_t) = delete;
template<class T, class V> auto operator-(not_null<T, V>, not_null<T, V>) = delete;
template<class T, class V> auto operator-(typename not_null<T, V>::pointer_type, not_null<T, V>) = delete;
template<class T, class V> auto operator-(not_null<T, V>, typename not_null<T, V>::pointer_type) = delete;



template<class T
	,class Validator>
class not_null<T[], Validator>
{
public:
	using pointer_type = T*;
	using element_type = T;
	using validator_type = Validator;

	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	constexpr
	not_null(U p) :
		m_ptr(p)
	{
		validator_type()(m_ptr);
	}

	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get())(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	not_null(const SmartPtr& ptr) :
		m_ptr(ptr.get())
	{
		validator_type()(m_ptr);
	}

	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	constexpr
	not_null&
	operator=(U rhs)
	{
		validator_type()(rhs);
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
	operator*() const noexcept
	{
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const noexcept
	{
		return m_ptr;
	}

	constexpr
	element_type&
	operator[](size_t idx) const noexcept
	{
		MTK_ASSERT(idx <= static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max()));
		return *(*this + static_cast<ptrdiff_t>(idx));
	}

#ifndef MTK_DOXYGEN
	template<class Integer
		,_require<std::is_integral_v<Integer>> = 0>
	constexpr
	element_type&
	operator[](Integer idx) const noexcept
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
	reset(U ptr)
	{
		validator_type()(ptr);
		m_ptr = ptr;
	}

	constexpr
	void
	swap(not_null& other) noexcept
	{
		auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}



	friend constexpr
	not_null&
	operator++(not_null& rhs) noexcept
	{
		++rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	not_null
	operator++(not_null& lhs, int) noexcept
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	not_null&
	operator--(not_null& rhs) noexcept
	{
		--rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	not_null
	operator--(not_null& lhs, int) noexcept
	{
		const auto cp = lhs;
		--lhs;
		return cp;
	}



	friend constexpr
	not_null&
	operator+=(not_null& lhs, ptrdiff_t rhs) noexcept
	{
		lhs.m_ptr += rhs;
		return lhs;
	}

	friend constexpr
	not_null
	operator+(not_null lhs, ptrdiff_t rhs) noexcept
	{
		lhs += rhs;
		return lhs;
	}

	friend constexpr
	not_null
	operator+(ptrdiff_t lhs, not_null rhs) noexcept
	{
		rhs += lhs;
		return rhs;
	}

	friend constexpr
	not_null&
	operator-=(not_null& lhs, ptrdiff_t rhs) noexcept
	{
		lhs.m_ptr -= rhs;
		return lhs;
	}

	friend constexpr
	not_null
	operator-(not_null lhs, ptrdiff_t rhs) noexcept
	{
		lhs -= rhs;
		return lhs;
	}

	friend constexpr
	ptrdiff_t
	operator-(not_null lhs, not_null rhs) noexcept
	{
		return (lhs.m_ptr - rhs.m_ptr);
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer_type lhs, not_null rhs) noexcept
	{
		return (pointer(lhs) - rhs);
	}

	friend constexpr
	ptrdiff_t
	operator-(not_null lhs, pointer_type rhs) noexcept
	{
		return (lhs - pointer(rhs));
	}

private:
	pointer_type m_ptr;
};

template<class T
	,class V>
constexpr
void
swap(not_null<T, V>& a, not_null<T, V>& b) noexcept
{
	a.swap(b);
}

} // namespace mtk

#endif
