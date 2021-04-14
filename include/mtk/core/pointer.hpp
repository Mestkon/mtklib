#ifndef MTK_CORE_POINTER_HPP
#define MTK_CORE_POINTER_HPP

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/pointer_validator.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <limits>
#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class pointer
{
public:
	using pointer_type = T*;
	using element_type = T;
	using validator_type = Validator;

	constexpr
	pointer() noexcept :
		m_ptr(nullptr)
	{ }

	constexpr
	pointer(pointer_type ptr) noexcept :
		m_ptr(ptr)
	{ }

	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get()), pointer_type>> = 0
#endif
	>
	constexpr
	pointer(const SmartPtr& ptr) noexcept(noexcept(ptr.get())) :
		m_ptr(ptr.get())
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
		validator_type()(m_ptr);
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
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
		mtk::_swap(m_ptr, other.m_ptr);
	}

private:
	pointer_type m_ptr;
};

template<class T, class V> auto operator++(pointer<T, V>&) = delete;
template<class T, class V> auto operator++(pointer<T, V>&, int) = delete;
template<class T, class V> auto operator--(pointer<T, V>&) = delete;
template<class T, class V> auto operator--(pointer<T, V>&, int) = delete;

template<class T, class V> auto operator+=(pointer<T, V>&, ptrdiff_t) = delete;
template<class T, class V> auto operator-=(pointer<T, V>&, ptrdiff_t) = delete;
template<class T, class V> auto operator+(pointer<T, V>, ptrdiff_t) = delete;
template<class T, class V> auto operator+(ptrdiff_t, pointer<T, V>) = delete;
template<class T, class V> auto operator-(pointer<T, V>, ptrdiff_t) = delete;
template<class T, class V> auto operator-(pointer<T, V>, pointer<T, V>) = delete;
template<class T, class V> auto operator-(typename pointer<T, V>::pointer_type, pointer<T, V>) = delete;
template<class T, class V> auto operator-(pointer<T, V>, typename pointer<T, V>::pointer_type) = delete;


template<class T
	,class Validator>
class pointer<T[], Validator>
{
public:
	using pointer_type = T*;
	using element_type = T;
	using validator_type = Validator;

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

	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get())(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	pointer(const SmartPtr& ptr) noexcept(noexcept(ptr.get())) :
		m_ptr(ptr.get())
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
		validator_type()(m_ptr);
		return *m_ptr;
	}

	constexpr
	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
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
		mtk::_swap(m_ptr, other.m_ptr);
	}



	friend constexpr
	pointer&
	operator++(pointer& rhs)
	{
		validator_type()(rhs.m_ptr);
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
		validator_type()(rhs.m_ptr);
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
		validator_type()(lhs.m_ptr);
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
		validator_type()(lhs.m_ptr);
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
		if (!lhs && !rhs)
			return ptrdiff_t(0);
		validator_type()(lhs.m_ptr);
		validator_type()(rhs.m_ptr);
		return (lhs.m_ptr - rhs.m_ptr);
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer_type lhs, pointer rhs)
	{
		return (pointer(lhs) - rhs);
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer lhs, pointer_type rhs)
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
swap(pointer<T, V>& a, pointer<T, V>& b) noexcept
{
	a.swap(b);
}

} // namespace mtk

#endif
