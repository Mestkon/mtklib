#ifndef MTK_CORE_GUARDED_PTR_HPP
#define MTK_CORE_GUARDED_PTR_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/pointer_validators.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class guarded_ptr :
	Validator
{
	static_assert(std::is_nothrow_default_constructible_v<Validator>, "Validator must be nothrow default constructible");

	static constexpr bool _nothrow_validator = std::is_nothrow_invocable_v<Validator, T*>;
	static constexpr bool _nothrow_validator_copy = std::is_nothrow_copy_constructible_v<Validator>;

public:
	using pointer = T*;
	using element_type = T;
	using validator_type = Validator;

	constexpr
	guarded_ptr(const Validator& v = Validator()) noexcept(_nothrow_validator_copy) :
		guarded_ptr(nullptr, v)
	{ }

	constexpr
	guarded_ptr(T* p, const Validator& v = Validator()) noexcept(_nothrow_validator_copy) :
		Validator(v),
		m_ptr(p)
	{ }

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_same_v<T, const U>> = 0
#endif
	>
	guarded_ptr(guarded_ptr<U, V> p, const Validator& v = Validator()) noexcept(_nothrow_validator_copy) :
		guarded_ptr(p.get(), v)
	{ }



	constexpr
	guarded_ptr&
	operator=(T* rhs) noexcept { m_ptr = rhs; return *this; }

	constexpr
	operator T*() const noexcept { return m_ptr; }

	constexpr
	element_type&
	operator*() const noexcept(_nothrow_validator) { this->_verify(); return *m_ptr; }

	constexpr
	pointer
	operator->() const noexcept(_nothrow_validator) { this->_verify(); return m_ptr; }

	constexpr
	element_type&
	operator[](std::size_t idx) const noexcept(_nothrow_validator) { this->_verify(); return *(m_ptr + idx); }

	pointer
	get() const noexcept { return m_ptr; }

	const validator_type&
	validator() const noexcept { return static_cast<const Validator&>(*this); }



	friend constexpr bool operator==(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr == rhs.m_ptr); }
	friend constexpr bool operator==(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr == rhs); }
	friend constexpr bool operator==(pointer lhs, guarded_ptr rhs) noexcept { return (lhs == rhs.m_ptr); }

	friend constexpr bool operator!=(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr != rhs.m_ptr); }
	friend constexpr bool operator!=(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr != rhs); }
	friend constexpr bool operator!=(pointer lhs, guarded_ptr rhs) noexcept { return (lhs != rhs.m_ptr); }

	friend constexpr bool operator<(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr < rhs.m_ptr); }
	friend constexpr bool operator<(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr < rhs); }
	friend constexpr bool operator<(pointer lhs, guarded_ptr rhs) noexcept { return (lhs < rhs.m_ptr); }

	friend constexpr bool operator>(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr > rhs.m_ptr); }
	friend constexpr bool operator>(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr > rhs); }
	friend constexpr bool operator>(pointer lhs, guarded_ptr rhs) noexcept { return (lhs > rhs.m_ptr); }

	friend constexpr bool operator<=(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr <= rhs.m_ptr); }
	friend constexpr bool operator<=(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr <= rhs); }
	friend constexpr bool operator<=(pointer lhs, guarded_ptr rhs) noexcept { return (lhs <= rhs.m_ptr); }

	friend constexpr bool operator>=(guarded_ptr lhs, guarded_ptr rhs) noexcept { return (lhs.m_ptr >= rhs.m_ptr); }
	friend constexpr bool operator>=(guarded_ptr lhs, pointer rhs) noexcept { return (lhs.m_ptr >= rhs); }
	friend constexpr bool operator>=(pointer lhs, guarded_ptr rhs) noexcept { return (lhs >= rhs.m_ptr); }



	friend constexpr
	guarded_ptr&
	operator++(guarded_ptr& rhs) noexcept(_nothrow_validator)
	{
		rhs._verify();
		++rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	guarded_ptr
	operator++(guarded_ptr& lhs, int) noexcept(_nothrow_validator)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	guarded_ptr&
	operator--(guarded_ptr& rhs) noexcept(_nothrow_validator)
	{
		rhs._verify();
		--rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	guarded_ptr
	operator--(guarded_ptr& lhs, int) noexcept(_nothrow_validator)
	{
		auto cp = lhs;
		--lhs;
		return cp;
	}



	template<class U>
	friend constexpr
#ifdef MTK_DOXYGEN
	guarded_ptr&
#else
	std::enable_if_t<std::is_integral_v<U>, guarded_ptr&>
#endif
	operator+=(guarded_ptr& lhs, U rhs) noexcept(_nothrow_validator)
	{
		lhs._verify();
		lhs.m_ptr += rhs;
		return lhs;
	}

	template<class U>
	friend constexpr
#ifdef MTK_DOXYGEN
	guarded_ptr&
#else
	std::enable_if_t<std::is_integral_v<U>, guarded_ptr&>
#endif
	operator-=(guarded_ptr& lhs, U rhs) noexcept(_nothrow_validator)
	{
		lhs._verify();
		lhs.m_ptr -= rhs;
		return lhs;
	}

	template<class U>
	friend constexpr
#ifdef MTK_DOXYGEN
	guarded_ptr
#else
	std::enable_if_t<std::is_integral_v<U>, guarded_ptr>
#endif
	operator+(guarded_ptr lhs, U rhs) noexcept(_nothrow_validator)
	{
		lhs += rhs;
		return lhs;
	}

	template<class U>
	friend constexpr
#ifdef MTK_DOXYGEN
	guarded_ptr
#else
	std::enable_if_t<std::is_integral_v<U>, guarded_ptr>
#endif
	operator+(U lhs, guarded_ptr rhs) noexcept(_nothrow_validator)
	{
		rhs += lhs;
		return rhs;
	}

	template<class U>
	friend constexpr
#ifdef MTK_DOXYGEN
	guarded_ptr
#else
	std::enable_if_t<std::is_integral_v<U>, guarded_ptr>
#endif
	operator-(guarded_ptr lhs, U rhs) noexcept(_nothrow_validator)
	{
		lhs -= rhs;
		return lhs;
	}

	friend constexpr
	ptrdiff_t
	operator-(guarded_ptr lhs, guarded_ptr rhs) noexcept(_nothrow_validator)
	{
		lhs._verify();
		rhs._verify();
		return (lhs.m_ptr - rhs.m_ptr);
	}

	friend  constexpr
	ptrdiff_t
	operator-(guarded_ptr lhs, pointer rhs) noexcept(_nothrow_validator)
	{
		lhs._verify();
		return (lhs.m_ptr - rhs);
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer lhs, guarded_ptr rhs) noexcept(_nothrow_validator)
	{
		rhs._verify();
		return (lhs - rhs.m_ptr);
	}

private:
	void _verify() const noexcept(_nothrow_validator)
	{
		this->validator()(m_ptr);
	}

	pointer m_ptr;
};

} // namespace mtk

#endif
