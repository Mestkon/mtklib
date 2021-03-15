#ifndef MTK_CORE_GUARDED_PTR_HPP
#define MTK_CORE_GUARDED_PTR_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/actor_base.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/pointer_validators.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class guarded_ptr :
	actor_base<Validator>
{
	static_assert(!std::is_reference_v<Validator>, "Validator cannot be a reference type");

	static constexpr bool _nothrow_validator_invoke = std::is_nothrow_invocable_v<Validator, T*>;
	static constexpr bool _nothrow_validator_default = std::is_nothrow_default_constructible_v<Validator>;
	static constexpr bool _nothrow_validator_copy = std::is_nothrow_copy_constructible_v<Validator>;
	static constexpr bool _nothrow_validator_move = std::is_nothrow_move_constructible_v<Validator>;

	using _type = std::remove_extent_t<T>;

public:
	using pointer = _type*;
	using element_type = _type;
	using validator_type = Validator;

#ifndef MTK_DOXYGEN
	template<class V = validator_type
		,_require<!std::is_pointer_v<V>> = 0
		,_require<std::is_default_constructible_v<V>> = 0>
#endif
	constexpr
	guarded_ptr() noexcept(_nothrow_validator_default && _nothrow_validator_copy) :
		guarded_ptr(nullptr, validator_type())
	{ }

	constexpr
	guarded_ptr(const validator_type& v) noexcept(_nothrow_validator_copy) :
		guarded_ptr(nullptr, v)
	{ }

	constexpr
	guarded_ptr(validator_type&& v) noexcept(_nothrow_validator_move) :
		guarded_ptr(nullptr, mtk::_move(v))
	{ }

	constexpr
	guarded_ptr(std::nullptr_t) noexcept(_nothrow_validator_default) :
		guarded_ptr(nullptr, validator_type())
	{ }

	template<class U
#ifndef MTK_DOXYGEN
		,_require<(!std::is_array_v<T> && std::is_convertible_v<U*, element_type*>)
			|| std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
		,class V = validator_type
		,_require<!std::is_pointer_v<V>> = 0
		,_require<std::is_default_constructible_v<V>> = 0
#endif
	>
	constexpr
	guarded_ptr(U* p) noexcept(_nothrow_validator_default && _nothrow_validator_copy) :
		guarded_ptr(p, validator_type())
	{ }

	template<class U
#ifndef MTK_DOXYGEN
	,_require<(!std::is_array_v<T> && std::is_convertible_v<U*, element_type*>)
			|| std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	guarded_ptr(U* p, const validator_type& v) noexcept(_nothrow_validator_copy) :
		actor_base<Validator>(v),
		m_ptr(p)
	{ }

	template<class U
#ifndef MTK_DOXYGEN
	,_require<(!std::is_array_v<T> && std::is_convertible_v<U*, element_type*>)
			|| std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	guarded_ptr(U* p, validator_type&& v) noexcept(_nothrow_validator_move) :
		actor_base<Validator>(mtk::_move(v)),
		m_ptr(p)
	{ }

	constexpr
	guarded_ptr(std::nullptr_t, const validator_type& v) noexcept(_nothrow_validator_copy) :
		actor_base<Validator>(v),
		m_ptr(nullptr)
	{ }

	constexpr
	guarded_ptr(std::nullptr_t, validator_type&& v) noexcept(_nothrow_validator_move) :
		actor_base<Validator>(mtk::_move(v)),
		m_ptr(nullptr)
	{ }

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,class Tp = T
		,_require<!std::is_array_v<Tp>> = 0
		,_require<!std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U*, pointer>> = 0
#endif
	>
	constexpr
	guarded_ptr(guarded_ptr<U, V> p) noexcept(_nothrow_validator_copy) :
		guarded_ptr(p.get(), p.get_validator())
	{ }

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,class Tp
		,_require<std::is_array_v<Tp>> = 0
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	guarded_ptr(guarded_ptr<U, V> p) noexcept(_nothrow_validator_copy) :
		guarded_ptr(p.get(), p.get_validator())
	{ }



	constexpr
	guarded_ptr&
	operator=(pointer rhs) noexcept { m_ptr = rhs; return *this; }

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,class Tp = T
		,_require<!std::is_array_v<Tp>> = 0
		,_require<!std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U*, pointer>> = 0
#endif
	>
	constexpr
	guarded_ptr&
	operator=(guarded_ptr<U, V> rhs) noexcept(_nothrow_validator_copy)
	{
		m_ptr = rhs.get();
		this->_get_actor() = rhs.get_validator();
		return *this;
	}

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,class Tp
		,_require<std::is_array_v<Tp>> = 0
		,_require<std::is_array_v<U>> = 0
		,_require<std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	guarded_ptr&
	operator=(guarded_ptr<U, V> rhs) noexcept(_nothrow_validator_copy)
	{
		m_ptr = rhs.get();
		this->_get_actor() = rhs.get_validator();
		return *this;
	}



	constexpr
	operator T*() const noexcept { return m_ptr; }

	constexpr
	element_type&
	operator*() const noexcept(_nothrow_validator_invoke) { this->_verify(); return *m_ptr; }

	constexpr
	pointer
	operator->() const noexcept(_nothrow_validator_invoke) { this->_verify(); return m_ptr; }

#ifndef MTK_DOXYGEN
	template<class U = T
		,_require<std::is_same_v<U, T>> = 0
		,_require<std::is_array_v<U>> = 0>
#endif
	constexpr
	element_type&
	operator[](size_t idx) const noexcept(_nothrow_validator_invoke) { this->_verify(); return *(m_ptr + idx); }

#ifndef MTK_DOXYGEN
	template<class U = T
		,_require<std::is_same_v<U, T>> = 0
		,_require<!std::is_array_v<U>> = 0>
#endif
	constexpr
	element_type&
	operator[](size_t idx) const noexcept = delete;



	template<class U
#ifndef MTK_DOXYGEN
	,_require<(!std::is_array_v<T> && std::is_convertible_v<U*, element_type*>)
			|| std::is_convertible_v<U(*)[], element_type(*)[]>> = 0
#endif
	>
	constexpr
	void
	reset(U* p) noexcept { m_ptr = p; }

	constexpr
	void
	reset(std::nullptr_t = nullptr) noexcept { m_ptr = nullptr; }

	pointer
	get() const noexcept { return m_ptr; }

	const validator_type&
	get_validator() const noexcept { return this->_get_actor(); }

	validator_type&
	get_validator() noexcept { return this->_get_actor(); }

	void
	swap(guarded_ptr& other) noexcept(std::is_nothrow_swappable_v<validator_type>)
	{
		using std::swap;
		swap(m_ptr, other.m_ptr);
		swap(this->get_validator(), other.get_validator());
	}



	friend constexpr
	guarded_ptr&
	operator++(guarded_ptr& rhs) noexcept(_nothrow_validator_invoke)
	{
		rhs._verify();
		++rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	guarded_ptr
	operator++(guarded_ptr& lhs, int) noexcept(_nothrow_validator_invoke)
	{
		auto cp = lhs;
		++lhs;
		return cp;
	}

	friend constexpr
	guarded_ptr&
	operator--(guarded_ptr& rhs) noexcept(_nothrow_validator_invoke)
	{
		rhs._verify();
		--rhs.m_ptr;
		return rhs;
	}

	friend constexpr
	guarded_ptr
	operator--(guarded_ptr& lhs, int) noexcept(_nothrow_validator_invoke)
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
	operator+=(guarded_ptr& lhs, U rhs) noexcept(_nothrow_validator_invoke)
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
	operator-=(guarded_ptr& lhs, U rhs) noexcept(_nothrow_validator_invoke)
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
	operator+(guarded_ptr lhs, U rhs) noexcept(_nothrow_validator_invoke)
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
	operator+(U lhs, guarded_ptr rhs) noexcept(_nothrow_validator_invoke)
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
	operator-(guarded_ptr lhs, U rhs) noexcept(_nothrow_validator_invoke)
	{
		lhs -= rhs;
		return lhs;
	}

	friend constexpr
	ptrdiff_t
	operator-(guarded_ptr lhs, guarded_ptr rhs) noexcept(_nothrow_validator_invoke)
	{
		lhs._verify();
		rhs._verify();
		return (lhs.m_ptr - rhs.m_ptr);
	}

	friend  constexpr
	ptrdiff_t
	operator-(guarded_ptr lhs, pointer rhs) noexcept(_nothrow_validator_invoke)
	{
		lhs._verify();
		return (lhs.m_ptr - rhs);
	}

	friend constexpr
	ptrdiff_t
	operator-(pointer lhs, guarded_ptr rhs) noexcept(_nothrow_validator_invoke)
	{
		rhs._verify();
		return (lhs - rhs.m_ptr);
	}

private:
	void _verify() const noexcept(_nothrow_validator_invoke)
	{
		this->get_validator()(m_ptr);
	}

	pointer m_ptr;
};



template<class T
	,class V>
void
swap(guarded_ptr<T, V>& a, guarded_ptr<T, V>& b) noexcept(noexcept(a.swap(b)))
{
	a.swap(b);
}

} // namespace mtk

#endif
