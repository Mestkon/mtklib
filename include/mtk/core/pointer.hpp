#ifndef MTK_CORE_POINTER_HPP
#define MTK_CORE_POINTER_HPP

//! @file
//! Contains mtk::pointer

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
//! before dereferencing the contained pointer.
//!
//! @code
//! #include <mtk/core/pointer.hpp>
//! @endcode
//!
//! Specializations:
//!	 - mtk::pointer<T[], Validator>
template<class T
	,class Validator = default_pointer_validator>
class pointer
{
public:
	//! typedef for T*.
	using pointer_type = T*;
	//! typedef for T.
	using element_type = T;
	//! typedef for Validator.
	using validator_type = Validator;

	//! Constructs a pointer initialized to nullptr.
	constexpr
	pointer() noexcept :
		m_ptr(nullptr)
	{ }

	//! Constructs a pointer initialized to ptr.
	constexpr
	pointer(pointer_type ptr) noexcept :
		m_ptr(ptr)
	{ }

	//! Constructs a pointer initialized to ptr.get().
	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get()), pointer_type>> = 0
#endif
	>
	constexpr
	pointer(const SmartPtr& ptr) noexcept(noexcept(ptr.get())) :
		m_ptr(ptr.get())
	{ }

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

	//! @brief Dereferences the stored pointer value. Invokes validator on get()
	//! before dereferencing.
	//!
	//! @pre If validation returns, the contained pointer must be a dereferencable pointer.
	constexpr
	element_type&
	operator*() const
	{
		validator_type()(m_ptr);
		return *m_ptr;
	}

	//! @brief Returns the stored pointer value. Invokes validator on get()
	//! before returning.
	//!
	//! @pre If validation returns, the contained pointer must be a dereferencable pointer.
	constexpr
	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
		return m_ptr;
	}

	template<class U>
	auto operator[](U&&) = delete;

	//! Replaces the contained pointer with ptr.
	constexpr
	void
	reset(pointer_type ptr = nullptr) noexcept
	{
		m_ptr = ptr;
	}

	//! Swaps the contained pointers.
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



//! @brief Specialization for T[].
//!
//! @code
//! #include <mtk/core/pointer.hpp>
//! @endcode
template<class T
	,class Validator>
class pointer<T[], Validator>
{
public:
	//! typedef for T*.
	using pointer_type = T*;
	//! typedef for T.
	using element_type = T;
	//! typedef for Validator.
	using validator_type = Validator;

	//! Constructs a pointer initialized to nullptr.
	constexpr
	pointer() noexcept :
		m_ptr(nullptr)
	{ }

	//! @brief Constructs a pointer initialized to p.
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
	pointer(U p) noexcept :
		m_ptr(p)
	{ }

	//! @brief Constructs a pointer initialized to ptr.get().
	//!
	//! @pre std::remove_reference_t<decltype(*ptr.get())>(*)[] must be convertible to value_type(*)[].
	template<class SmartPtr
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<decltype(mtk::_declval<const SmartPtr&>().get())(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	pointer(const SmartPtr& ptr) noexcept(noexcept(ptr.get())) :
		m_ptr(ptr.get())
	{ }

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

	//! @brief Dereferences the stored pointer value. Invokes validator on get()
	//! before dereferencing.
	//!
	//! @pre If validation returns, the contained pointer must be a dereferencable pointer.
	constexpr
	element_type&
	operator*() const
	{
		validator_type()(m_ptr);
		return *m_ptr;
	}

	//! @brief Returns the stored pointer value. Invokes validator on get()
	//! before returning.
	//!
	//! @pre If validation returns, the contained pointer must be a dereferencable pointer.
	constexpr
	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
		return m_ptr;
	}

	//! @brief Returns get()[idx].
	//!
	//! @pre idx must be a valid index for the stored array.
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

	//! @brief Replaces the contained pointer with ptr.
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
	reset(U ptr = nullptr) noexcept
	{
		m_ptr = ptr;
	}

	//! Swaps the contained pointers.
	constexpr
	void
	swap(pointer& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
	}



	//! @brief Increments the pointer in rhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer&
	operator++(pointer& rhs)
	{
		validator_type()(rhs.m_ptr);
		++rhs.m_ptr;
		return rhs;
	}

	//! @brief Increments the pointer in rhs and returns the previous value.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer
	operator++(pointer& lhs, int)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	//! @brief Decrements the pointer in rhs.
	//!
	//! @pre The resulting pointer must be a valid pointer.
	friend constexpr
	pointer&
	operator--(pointer& rhs)
	{
		validator_type()(rhs.m_ptr);
		--rhs.m_ptr;
		return rhs;
	}

	//! @brief Decrements the pointer in rhs and returns the previous value.
	//!
	//! @pre The resulting pointer must be a valid pointer.
	friend constexpr
	pointer
	operator--(pointer& lhs, int)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}



	//! @brief Adds rhs to the pointer in lhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer&
	operator+=(pointer& lhs, ptrdiff_t rhs)
	{
		validator_type()(lhs.m_ptr);
		lhs.m_ptr += rhs;
		return lhs;
	}

	//! @brief Returns a copy of lhs with rhs added to it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer
	operator+(pointer lhs, ptrdiff_t rhs)
	{
		lhs += rhs;
		return lhs;
	}

	//! @brief Returns a copy of rhs with lhs added to it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer
	operator+(ptrdiff_t lhs, pointer rhs)
	{
		rhs += lhs;
		return rhs;
	}

	//! @brief Subtracts rhs from the pointer in lhs.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer&
	operator-=(pointer& lhs, ptrdiff_t rhs)
	{
		validator_type()(lhs.m_ptr);
		lhs.m_ptr -= rhs;
		return lhs;
	}

	//! @brief Returns a copy of lhs with rhs subtracted from it.
	//!
	//! @pre The resulting pointer must be a valid pointer or end of range.
	friend constexpr
	pointer
	operator-(pointer lhs, ptrdiff_t rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	//! @brief Returns the difference between lhs.get() and rhs.get().
	//!
	//! @pre The pointers must be pointers to the same array or both must be nullptr.
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

//! @}

//! @brief Swaps the contained pointers.
//!
//! @relates pointer
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
