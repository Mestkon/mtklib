#ifndef MTK_CORE_NOT_NULLPTR_HPP
#define MTK_CORE_NOT_NULLPTR_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/pointer_validators.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class T
	,class Validator = default_pointer_validator>
class not_nullptr :
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
	not_nullptr(pointer p, const Validator& v = Validator()) noexcept(_nothrow_validator_copy && _nothrow_validator) :
		Validator(v),
		m_ptr(p)
	{
		this->_verify(m_ptr);
	}

	template<class U
		,class V
#ifndef MTK_DOXYGEN
		,_require<std::is_convertible_v<U*, T*>> = 0
#endif
	>
	not_nullptr(not_nullptr<U, V> p, const Validator& v = Validator()) noexcept(_nothrow_validator_copy) :
		Validator(v),
		m_ptr(p.get())
	{ }



	constexpr
	not_nullptr&
	operator=(pointer rhs) noexcept { this->_verify(rhs); m_ptr = rhs; return *this; }

	constexpr
	operator T*() const noexcept { return m_ptr; }

	constexpr
	element_type&
	operator*() const noexcept { return *m_ptr; }

	constexpr
	pointer
	operator->() const noexcept { return m_ptr; }

	constexpr
	element_type&
	operator[](std::size_t idx) const noexcept { return *(m_ptr + idx); }



	pointer
	get() const noexcept { return m_ptr; }

	const validator_type&
	validator() const noexcept { return static_cast<const Validator&>(*this); }

private:
	void _verify(pointer p) const noexcept(_nothrow_validator)
	{
		this->validator()(p);
	}


	pointer m_ptr;
};

} // namespace mtk

#endif
