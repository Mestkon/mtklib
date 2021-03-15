#ifndef MTK_CORE_IMPL_ACTOR_BASE_HPP
#define MTK_CORE_IMPL_ACTOR_BASE_HPP

#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

template<class Func
	,bool IsClass = std::is_class_v<Func>>
class actor_base
{ };

template<class Func>
class actor_base<Func, true> :
	Func
{
public:
	template<class F
		,_require<std::is_constructible_v<Func, F&&>> = 0>
	constexpr
	actor_base(F&& f) noexcept(std::is_nothrow_constructible_v<Func, F&&>) :
		Func(mtk::_forward<F>(f))
	{ }

	template<class F
		,_require<std::is_assignable_v<Func, F&&>> = 0>
	constexpr
	actor_base&
	operator=(F&& rhs) noexcept(std::is_nothrow_assignable_v<Func, F&&>)
	{
		static_cast<Func&>(*this) = mtk::_forward<F>(rhs);
		return *this;
	}

	constexpr
	Func&
	_get_actor() noexcept { return *this; }

	constexpr
	const Func&
	_get_actor() const noexcept { return *this; }
};

template<class Func>
class actor_base<Func, false>
{
public:
	template<class F
		,_require<std::is_constructible_v<Func, F&&>> = 0>
	constexpr
	actor_base(F&& f) noexcept(std::is_nothrow_constructible_v<Func, F&&>) :
		m_func(mtk::_forward<F>(f))
	{ }

	template<class F
		,_require<std::is_assignable_v<Func, F&&>> = 0>
	constexpr
	actor_base&
	operator=(F&& rhs) noexcept(std::is_nothrow_assignable_v<Func, F&&>)
	{
		m_func = mtk::_forward<F>(rhs);
		return *this;
	}

	constexpr
	Func&
	_get_actor() noexcept { return m_func; }

	constexpr
	const Func&
	_get_actor() const noexcept { return m_func; }

private:
	Func m_func;
};

} // namespace mtk

#endif
