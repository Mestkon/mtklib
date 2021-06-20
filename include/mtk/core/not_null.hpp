#ifndef MTK_CORE_NOT_NULL_HPP
#define MTK_CORE_NOT_NULL_HPP

//! @file
//! Contains mtk::not_null

#include <mtk/core/assert.hpp>
#include <mtk/core/types.hpp>
#include <mtk/core/impl/declval.hpp>
#include <mtk/core/impl/pointer_validator.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <limits>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Pointer-like class which invokes the validator
//! before assigning to the contained pointer.
//!
//! @code
//! #include <mtk/core/not_null.hpp>
//! @endcode
//!
//! Specializations:
//!  - mtk::not_null<T[], Validator>
template<class T
	,class Validator = default_pointer_validator>
class not_null
{
public:
	//! typedef.
	using pointer_type = T*;
	//! typedef.
	using element_type = T;
	//! typedef.
	using validator_type = Validator;

	//! Constructs a not_null initialized to ptr. Invokes validator on ptr.
	constexpr
	not_null(pointer_type ptr) :
		m_ptr(ptr)
	{
		validator_type()(m_ptr);
	}

	//! Constructs a not_null initialized to ptr.get(). Invokes validator on ptr.get().
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

	//! Non-explicit conversion to pointer_type.
	constexpr
	operator pointer_type() const noexcept
	{
		return m_ptr;
	}

	//! Returns bool(get()).
	explicit constexpr
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	//! Returns the stored pointer value.
	constexpr
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	//! Dereferences the stored pointer value.
	constexpr
	element_type&
	operator*() const noexcept
	{
		return *m_ptr;
	}

	//! Returns the stored pointer value.
	constexpr
	pointer_type
	operator->() const noexcept
	{
		return m_ptr;
	}

	template<class U>
	auto operator[](U&&) = delete;

	//! Invokes the validator on ptr and stores it.
	constexpr
	void
	reset(pointer_type ptr)
	{
		validator_type()(ptr);
		m_ptr = ptr;
	}

	//! Swaps the contained pointers.
	constexpr
	void
	swap(not_null& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
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



//! @brief Specialization for T[].
//!
//! @code
//! #include <mtk/core/not_null.hpp>
//! @endcode
template<class T
	,class Validator>
class not_null<T[], Validator>
{
public:
	//! typedef.
	using pointer_type = T*;
	//! typedef.
	using element_type = T;
	//! typedef.
	using validator_type = Validator;

	//! @brief Constructs a not_null initialized to p. Invokes validator on p.
	//!
	//! @pre U must be std::nullptr_t or
	//! @pre U must be a pointer type U2* and U2(*)[] must be convertible to value_type(*)[].
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

	//! @brief Constructs a not_null initialized to ptr.get(). Invokes validator on ptr.get().
	//!
	//! @pre std::remove_reference_t<decltype(*ptr.get())>(*)[] must be convertible to value_type(*)[].
	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<std::remove_reference_t<decltype(*mtk::_declval<const SmartPtr&>().get())>(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	not_null(const SmartPtr& ptr) :
		m_ptr(ptr.get())
	{
		validator_type()(m_ptr);
	}

	//! Non-explicit conversion to pointer_type.
	constexpr
	operator pointer_type() const noexcept
	{
		return m_ptr;
	}

	//! Returns bool(get()).
	explicit constexpr
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	//! Returns the stored pointer value.
	constexpr
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	//! Dereferences the stored pointer value.
	constexpr
	element_type&
	operator*() const noexcept
	{
		return *m_ptr;
	}

	//! Retruns the stored pointer value.
	constexpr
	pointer_type
	operator->() const noexcept
	{
		return m_ptr;
	}

	//! @brief Returns get()[idx].
	//!
	//! @pre idx must be a valid index for the stored array.
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

	//! @brief Invokes the validator on ptr and stores it.
	//!
	//! @pre U must be std::nullptr_t or
	//! @pre U must be a pointer type U2* and U2(*)[] must be convertible to value_type(*)[].
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

	//! Swaps the contained pointers.
	constexpr
	void
	swap(not_null& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
	}



	//! @brief Increments the pointer in rhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null&
	operator++(not_null& rhs) noexcept
	{
		++rhs.m_ptr;
		return rhs;
	}

	//! @brief Increments the pointer in rhs and returns the previous value.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null
	operator++(not_null& lhs, int) noexcept
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	//! @brief Decrements the pointer in rhs.
	//!
	//! @pre The resulting pointer must be a valid pointer.
	friend constexpr
	not_null&
	operator--(not_null& rhs) noexcept
	{
		--rhs.m_ptr;
		return rhs;
	}

	//! @brief Decrements the pointer in rhs and returns the previous value.
	//!
	//! @pre The resulting pointer must be a valid pointer.
	friend constexpr
	not_null
	operator--(not_null& lhs, int) noexcept
	{
		const auto cp = lhs;
		--lhs;
		return cp;
	}



	//! @brief Adds rhs to the pointer in lhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null&
	operator+=(not_null& lhs, ptrdiff_t rhs) noexcept
	{
		lhs.m_ptr += rhs;
		return lhs;
	}

	//! @brief Returns a copy of lhs with rhs added to it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null
	operator+(not_null lhs, ptrdiff_t rhs) noexcept
	{
		lhs += rhs;
		return lhs;
	}

	//! @brief Returns a copy of rhs with lhs added to it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null
	operator+(ptrdiff_t lhs, not_null rhs) noexcept
	{
		rhs += lhs;
		return rhs;
	}

	//! @brief Subtracts rhs from the pointer in lhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null&
	operator-=(not_null& lhs, ptrdiff_t rhs) noexcept
	{
		lhs.m_ptr -= rhs;
		return lhs;
	}

	//! @brief Returns a copy of lhs with rhs subtracted from it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	not_null
	operator-(not_null lhs, ptrdiff_t rhs) noexcept
	{
		lhs -= rhs;
		return lhs;
	}

	//! @brief Returns the difference between lhs.get() and rhs.get().
	//!
	//! @pre The pointers must be pointers to the same array.
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

//! @}

//! @brief Swaps the contained pointers.
//!
//! @relates not_null
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
