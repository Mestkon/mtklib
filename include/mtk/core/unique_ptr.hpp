#ifndef MTK_CORE_UNIQUE_PTR_HPP
#define MTK_CORE_UNIQUE_PTR_HPP

//! @file
//! Contains mtk::unique_ptr

#include <mtk/core/types.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/pointer_validator.hpp>
#include <mtk/core/impl/require.hpp>
#include <mtk/core/impl/swap.hpp>

#include <cstddef>
#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Lightweight unique_ptr implementation
//! with validator and no custom deleter.
//!
//! @code
//! #include <mtk/core/unique_ptr.hpp>
//! @endcode
//!
//! Validator is invoked before dereferencing the contained pointer.
//!
//! Specializations:
//!	 - mtk::unique_ptr<T[], Validator>
template<class T
	,class Validator = default_pointer_validator>
class unique_ptr
{
public:
	//! typedef.
	using pointer_type = T*;
	//! typedef.
	using pointer = pointer_type;
	//! typedef.
	using element_type = T;
	//! typedef.
	using validator_type = Validator;

	//! Constructs a unique_ptr initialized to nullptr.
	unique_ptr() noexcept :
		m_ptr(nullptr)
	{ }

	//! Constructs a unique_ptr initialized to nullptr.
	unique_ptr(std::nullptr_t) noexcept :
		m_ptr(nullptr)
	{ }

	//! Constructs a unique_ptr initialized to ptr.
	explicit
	unique_ptr(pointer_type ptr) noexcept :
		m_ptr(ptr)
	{ }

	//! Deleted.
	unique_ptr(const unique_ptr&) = delete;

	//! Move constructor.
	unique_ptr(unique_ptr&& other) noexcept :
		m_ptr(other.release())
	{ }

	//! @brief Templated move constructor.
	//!
	//! @pre U must not be an array type.
	//! @pre U* must be convertible to T*.
	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<!std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U*, T*>> = 0
#endif
	>
	unique_ptr(unique_ptr<U, V>&& other) noexcept :
		m_ptr(other.release())
	{ }

	//! Deletes the managed object.
	~unique_ptr() noexcept
	{
		if (m_ptr)
			delete m_ptr;
	}

	//! Deleted.
	unique_ptr&
	operator=(const unique_ptr&) = delete;

	//! Move assignment.
	unique_ptr&
	operator=(unique_ptr&& rhs) noexcept
	{
		if (m_ptr)
			delete m_ptr;

		m_ptr = rhs.release();
		return *this;
	}

	//! @brief Templated move assignment.
	//!
	//! @pre U must not be an array type.
	//! @pre U* must be convertible to T*.
	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<!std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U*, T*>> = 0
#endif
	>
	unique_ptr&
	operator=(unique_ptr<U, V>&& rhs) noexcept
	{
		if (m_ptr)
			delete m_ptr;

		m_ptr = rhs.release();
		return *this;
	}

	//! Returns bool(get()).
	explicit
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	//! Returns the stored pointer.
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	//! @brief Dereferences the stored pointer. Invokes validator on get()
	//! before dereferencing.
	//!
	//! @pre If validation returns, the contained pointer must be dereferencable.
	element_type&
	operator*() const
	{
		validator_type()(m_ptr);
		return *m_ptr;
	}

	//! @brief Returns the stored pointer. Invokes validator on get()
	//! before returning.
	//!
	//! @pre If validation returns, the contained pointer must be dereferencable.
	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
		return m_ptr;
	}

	//! @brief Releases the ownership of the managed object.
	//!
	//! The caller is responsible for deleting this object.
	pointer_type
	release() noexcept
	{
		return mtk::_exchange(m_ptr, nullptr);
	}

	//! Replaces the contained pointer with ptr.
	void
	reset(pointer_type ptr = nullptr) noexcept
	{
		if (m_ptr)
			delete m_ptr;

		m_ptr = ptr;
	}

	//! Swaps the contained pointers.
	void
	swap(unique_ptr& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
	}

private:
	pointer_type m_ptr;
};



//! @brief Specialization for T[].
//!
//! @code
//! #include <mtk/core/unique_ptr.hpp>
//! @endcode
template<class T
	,class Validator>
class unique_ptr<T[], Validator>
{
public:
	//! typedef.
	using pointer_type = T*;
	//! typedef.
	using pointer = pointer_type;
	//! typedef.
	using element_type = T;
	//! typedef.
	using validator_type = Validator;

	//! Constructs a unique_ptr initialized to nullptr.
	unique_ptr() noexcept :
		m_ptr(nullptr)
	{ }

	//! Constructs a unique_ptr initialized to nullptr.
	unique_ptr(std::nullptr_t) noexcept :
		m_ptr(nullptr)
	{ }

	//! @brief Constructs a unique_ptr initialized to ptr.
	//!
	//! @pre U must be nullptr_t or
	//! @pre U must be a pointer V* and V(*)[] must be convertible to element_type(*)[].
	template<class U
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<U, std::nullptr_t> ||
			(std::is_pointer_v<U> && std::is_convertible_v<std::remove_pointer_t<U>(*)[], element_type(*)[]>)> = 0
#endif
	>
	explicit
	unique_ptr(U ptr) noexcept :
		m_ptr(ptr)
	{ }

	//! Deleted.
	unique_ptr(const unique_ptr&) = delete;

	//! Move constructor.
	unique_ptr(unique_ptr&& other) noexcept :
		m_ptr(other.release())
	{ }

	//! @brief Templated move constructor.
	//!
	//! @pre U must be an array_type.
	//! @pre unique_ptr<U, V>::element_type(*)[] must be convertible to element_type(*)[].
	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<typename unique_ptr<U, V>::element_type(*)[], element_type(*)[]>> = 0
#endif
	>
	unique_ptr(unique_ptr<U, V>&& other) noexcept :
		m_ptr(other.release())
	{ }

	//! Deletes the managed object.
	~unique_ptr() noexcept
	{
		if (m_ptr)
			delete[] m_ptr;
	}

	//! Deleted.
	unique_ptr&
	operator=(const unique_ptr&) = delete;

	//! Move assignment.
	unique_ptr&
	operator=(unique_ptr&& rhs) noexcept
	{
		if (m_ptr)
			delete[] m_ptr;

		m_ptr = rhs.release();
		return *this;
	}

	//! @brief Templated move assignment.
	//!
	//! @pre U must be an array type.
	//! @pre unique_ptr<U, V>::element_type(*)[] must be convertible to element_type(*)[].
	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<typename unique_ptr<U, V>::element_type(*)[], element_type(*)[]>> = 0
#endif
	>
	unique_ptr&
	operator=(unique_ptr<U, V>&& rhs) noexcept
	{
		if (m_ptr)
			delete[] m_ptr;

		m_ptr = rhs.release();
	}

	//! Returns bool(get()).
	explicit
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	//! Returns the stored pointer.
	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	//! @brief Returns get()[idx].
	//!
	//! @pre idx must be a valid index for the stored array.
	element_type&
	operator[](size_t idx) const
	{
		validator_type()(m_ptr);
		return *(m_ptr + idx);
	}

	//! @brief Releases the ownership of the managed object.
	//!
	//! The caller is responsible for deleting this object.
	pointer_type
	release() noexcept
	{
		return mtk::_exchange(m_ptr, nullptr);
	}

	//! @brief Replaces the contained pointer with ptr.
	//!
	//! @pre U must be nullptr_t or
	//! @pre U must be a pointer type V* and V(*)[] must be convertible to element_type(*)[].
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
		if (m_ptr)
			delete[] m_ptr;

		m_ptr = ptr;
	}

	//! Swaps the contained pointers.
	void
	swap(unique_ptr& other) noexcept
	{
		mtk::_swap(m_ptr, other.m_ptr);
	}

private:
	pointer_type m_ptr;
};

//! @brief Constructs an object of type T and wraps it in unique_ptr.
//!
//! @pre T must not be an array type.
//!
//! @relates unique_ptr
template<class T
	,class... Args
#ifndef MTK_DOXYGEN
	,_require<!std::is_array_v<T>> = 0
#endif
>
unique_ptr<T>
make_unique(Args&& ...args)
{
	return unique_ptr<T>(new T(mtk::_forward<Args>(args)...));
}

//! @brief Constructs an array of objects of type T and size n and wraps
//! it in unique_ptr.
//!
//! @pre T must be an array type.
//!
//! @relates unique_ptr
template<class T
#ifndef MTK_DOXYGEN
	,_require<std::is_array_v<T>> = 0
#endif
>
unique_ptr<T>
make_unique(size_t n)
{
	return unique_ptr<T>(new std::remove_extent_t<T>[n]());
}

//! @}

//! @brief Swaps the contained pointers.
//!
//! @relates unique_ptr
template<class T
	,class V>
void
swap(unique_ptr<T, V>& a, unique_ptr<T, V>& b) noexcept
{
	a.swap(b);
}

//! @brief Returns lhs.get() == rhs.get()
//!
//! @relates unique_ptr.
template<class T
	,class V1
	,class V2>
bool
operator==(const unique_ptr<T, V1>& lhs, const unique_ptr<T, V2>& rhs) noexcept
{
	return (lhs.get() == rhs.get());
}

//! @brief Returns lhs.get() != rhs.get()
//!
//! @relates unique_ptr.
template<class T
	,class V1
	,class V2>
bool
operator!=(const unique_ptr<T, V1>& lhs, const unique_ptr<T, V2>& rhs) noexcept
{
	return (lhs.get() != rhs.get());
}

} // namespace mtk

#endif
