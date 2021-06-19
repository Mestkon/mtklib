#ifndef MTK_CORE_SCOPE_EXIT_HPP
#define MTK_CORE_SCOPE_EXIT_HPP

//! @file
//! Contains mtk::scope_exit, mtk::scope_success, and mtk::scope_failure

#include <mtk/core/impl/exception.hpp>
#include <mtk/core/impl/move.hpp>
#include <mtk/core/impl/require.hpp>

#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

//! @brief Invokes the given functor when object goes out of scope.
//!
//! @code
//! #include <mtk/core/scope_exit.hpp>
//! @endcode
//!
//! @pre Func must be invocable with zero arguments.
template<class Func>
class scope_exit
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	//! @brief Constructs a scope_exit with the given functor.
	//!
	//! @pre Func must be constructible from F.
	template<class F
#ifndef MTK_DOXYGEN
		,_require<std::is_constructible_v<Func, F>> = 0
#endif
	>
	constexpr
	scope_exit(F&& f) :
		m_func(mtk::_forward<F>(f)),
		m_active(true)
	{ }

	//! Deleted.
	scope_exit(const scope_exit&) = delete;

	//! Invokes the given functor unless release() has been invoked.
	~scope_exit() noexcept
	{
		if (m_active)
			m_func();
	}

	//! Deleted.
	auto operator=(const scope_exit&) = delete;

	//! Disables the scope_exit.
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

//! Deduction guide.
template<class Func>
scope_exit(Func) -> scope_exit<Func>;



//! @brief Invokes the given functor when object goes out of scope
//! and no exceptions are currently propagating.
//!
//! @code
//! #include <mtk/core/scope_exit.hpp>
//! @endcode
//!
//! @pre Func must be invocable with zero arguments.
template<class Func>
class scope_success
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	//! @brief Constructs a scope_success with the given functor.
	//!
	//! @pre Func must be constructible from F.
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

	//! Deleted.
	scope_success(const scope_success&) = delete;

	//! @brief Invokes the given functor if no exeptions are currently
	//! propagating, unless release() has been invoked.
	~scope_success() noexcept
	{
		const bool has_ex = mtk::_uncaught_exceptions();
		if (m_active && !has_ex)
			m_func();
	}

	//! Deleted.
	auto operator=(const scope_success&) = delete;

	//! Disables the scope_success.
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

//! Deduction guide.
template<class Func>
scope_success(Func) -> scope_success<Func>;



//! @brief Invokes the given functor when object goes out of scope and at least
//! one exception is currently propagating.
//!
//! @code
//! #include <mtk/core/scope_exit.hpp>
//! @endcode
//!
//! @pre Func must be invocable with zero arguments.
template<class Func>
class scope_failure
{
	static_assert(std::is_invocable_v<Func>, "Func must be invocable with zero arguments");
public:
	//! @brief Constructs a scope_failure with the given functor.
	//!
	//! @pre Func must be constructible from F.
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

	//! Deleted.
	scope_failure(const scope_failure&) = delete;

	//! @brief Invokes the given functor if at least one exeption are currently
	//! propagating, unless release() has been invoked.
	~scope_failure() noexcept
	{
		const bool has_ex = mtk::_uncaught_exceptions();
		if (m_active && has_ex)
			m_func();
	}

	//! Deleted.
	auto operator=(const scope_failure&) = delete;

	//! Disables the scope_failure.
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

//! Deduction guide.
template<class Func>
scope_failure(Func) -> scope_failure<Func>;

//! @}

} // namespace mtk

#endif
