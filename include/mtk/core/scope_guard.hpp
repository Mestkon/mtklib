#ifndef MTK_CORE_SCOPE_GUARD_HPP
#define MTK_CORE_SCOPE_GUARD_HPP

#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {
namespace impl_scope_guard {

bool
_uncaught_exceptions() noexcept;

} // namespace impl_scope_guard



template<class Func>
class scope_guard
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	template<class F
#ifndef MTK_DOXYGEN
		,_require<std::is_constructible_v<Func, F>> = 0
#endif
	>
	constexpr
	scope_guard(F&& f) :
		m_func(mtk::_forward<F>(f)),
		m_active(true)
	{ }

	scope_guard(const scope_guard&) = delete;

	~scope_guard() noexcept
	{
		if (m_active)
			m_func();
	}

	auto operator=(const scope_guard&) = delete;

	constexpr
	void
	release() noexcept
	{
		m_active = false;
	}

private:
	Func m_func;
	bool m_active;
};

template<class Func>
scope_guard(Func) -> scope_guard<Func>;



template<class Func>
class scope_success
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	template<class F
#ifndef MTK_DOXYGEN
		,_require<std::is_constructible_v<Func, F>> = 0
#endif
	>
	constexpr
	scope_success(F&& f) :
		m_func(mtk::_forward<F>(f)),
		m_active(true)
	{ }

	scope_success(const scope_success&) = delete;

	~scope_success() noexcept
	{
		const bool has_ex = mtk::impl_scope_guard::_uncaught_exceptions();
		if (m_active && !has_ex)
			m_func();
	}

	auto operator=(const scope_success&) = delete;

	constexpr
	void
	release() noexcept
	{
		m_active = false;
	}

private:
	Func m_func;
	bool m_active;
};

template<class Func>
scope_success(Func) -> scope_success<Func>;



template<class Func>
class scope_failure
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	template<class F
#ifndef MTK_DOXYGEN
		,_require<std::is_constructible_v<Func, F>> = 0
#endif
	>
	constexpr
	scope_failure(F&& f) :
		m_func(mtk::_forward<F>(f)),
		m_active(true)
	{ }

	scope_failure(const scope_failure&) = delete;

	~scope_failure() noexcept
	{
		const bool has_ex = mtk::impl_scope_guard::_uncaught_exceptions();
		if (m_active && has_ex)
			m_func();
	}

	auto operator=(const scope_failure&) = delete;

	constexpr
	void
	release() noexcept
	{
		m_active = false;
	}

private:
	Func m_func;
	bool m_active;
};

template<class Func>
scope_failure(Func) -> scope_failure<Func>;

} // namespace mtk

#endif
