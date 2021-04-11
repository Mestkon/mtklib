#ifndef MTK_CORE_UNIQUE_PTR_HPP
#define MTK_CORE_UNIQUE_PTR_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/pointer_validator.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class unique_ptr
{
public:
	using pointer_type = T*;
	using pointer = pointer_type;
	using element_type = T;
	using validator_type = Validator;

	unique_ptr() noexcept :
		m_ptr(nullptr)
	{ }

	explicit
	unique_ptr(pointer_type ptr) noexcept :
		m_ptr(ptr)
	{ }

	unique_ptr(const unique_ptr&) = delete;

	unique_ptr(unique_ptr&& other) noexcept :
		m_ptr(other.release())
	{ }

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

	~unique_ptr() noexcept
	{
		if (m_ptr)
			delete m_ptr;
	}

	unique_ptr&
	operator=(const unique_ptr&) = delete;

	unique_ptr&
	operator=(unique_ptr&& rhs) noexcept
	{
		if (m_ptr)
			delete m_ptr;

		m_ptr = rhs.release();
		return *this;
	}

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

	explicit
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	element_type&
	operator*() const
	{
		validator_type()(m_ptr);
		return *m_ptr;
	}

	pointer_type
	operator->() const
	{
		validator_type()(m_ptr);
		return m_ptr;
	}

	pointer_type
	release() noexcept
	{
		auto p = m_ptr;
		m_ptr = nullptr;
		return p;
	}

	void
	reset(pointer_type ptr = nullptr) noexcept
	{
		if (m_ptr)
			delete m_ptr;

		m_ptr = ptr;
	}

	void
	swap(unique_ptr& other) noexcept
	{
		auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}

private:
	pointer_type m_ptr;
};

template<class T
	,class Validator>
class unique_ptr<T[], Validator>
{
public:
	using pointer_type = T*;
	using pointer = pointer_type;
	using element_type = T;
	using validator_type = Validator;

	unique_ptr() noexcept :
		m_ptr(nullptr)
	{ }

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

	unique_ptr(const unique_ptr&) = delete;

	unique_ptr(unique_ptr&& other) noexcept :
		m_ptr(other.release())
	{ }

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<typename unique_ptr<U, V>::pointer_type(*)[], element_type(*)[]>> = 0
#endif
	>
	unique_ptr(unique_ptr<U, V>&& other) noexcept :
		m_ptr(other.release())
	{ }

	~unique_ptr() noexcept
	{
		if (m_ptr)
			delete[] m_ptr;
	}

	unique_ptr&
	operator=(const unique_ptr&) = delete;

	unique_ptr&
	operator=(unique_ptr&& rhs) noexcept
	{
		if (m_ptr)
			delete[] m_ptr;

		m_ptr = rhs.release();
		return *this;
	}

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<typename unique_ptr<U, V>::pointer_type(*)[], element_type(*)[]>> = 0
#endif
	>
	unique_ptr&
	operator=(unique_ptr<U, V>&& rhs) noexcept
	{
		if (m_ptr)
			delete[] m_ptr;

		m_ptr = rhs.release();
	}

	explicit
	operator bool() const noexcept
	{
		return bool(m_ptr);
	}

	pointer_type
	get() const noexcept
	{
		return m_ptr;
	}

	element_type&
	operator[](size_t idx) const
	{
		validator_type()(m_ptr);
		return *(m_ptr + idx);
	}

	pointer_type
	release() noexcept
	{
		auto p = m_ptr;
		m_ptr = nullptr;
		return p;
	}

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

	void
	swap(unique_ptr& other) noexcept
	{
		auto p = m_ptr;
		m_ptr = other.m_ptr;
		other.m_ptr = p;
	}

private:
	pointer_type m_ptr;
};

template<class T
	,class V>
void
swap(unique_ptr<T, V>& a, unique_ptr<T, V>& b) noexcept
{
	a.swap(b);
}

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

template<class T
	,class V1
	,class V2>
bool
operator==(unique_ptr<T, V1>& lhs, unique_ptr<T, V2>& rhs) noexcept
{
	return (lhs.get() == rhs.get());
}
template<class T
	,class V1
	,class V2>
bool
operator!=(unique_ptr<T, V1>& lhs, unique_ptr<T, V2>& rhs) noexcept
{
	return (lhs.get() != rhs.get());
}

} // namespace mtk

#endif
