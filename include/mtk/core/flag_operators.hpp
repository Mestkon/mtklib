#ifndef MTK_CORE_FLAG_OPERATORS_HPP
#define MTK_CORE_FLAG_OPERATORS_HPP

//! @file Contains MTK_DEFINE_FLAG_OPERATORS

#include <mtk/core/preprocessor.hpp>

#include <type_traits>

namespace mtk {

//! @addtogroup core
//! @{

#define MTK_IMPL_FLAG_OPERATOR(ENUM, OP) \
	constexpr \
	ENUM \
	operator OP (ENUM lhs, ENUM rhs) noexcept \
	{ \
		using _ut = std::underlying_type_t<ENUM>; \
		return static_cast<ENUM>(static_cast<_ut>(lhs) OP static_cast<_ut>(rhs)); \
	} \
	constexpr \
	ENUM& \
	operator MTK_CONCAT(OP, =) (ENUM& lhs, ENUM rhs) noexcept \
	{ \
		lhs = (lhs OP rhs); \
		return lhs; \
	}

//! @brief Adds flag operators to ENUM.
//!
//! @code
//! #include <mtk/core/flag_operators.hpp>
//! @endcode
//!
//! Defines the operators for ENUM:
//!	 - ENUM& operator|=(ENUM&, ENUM)
//!  - ENUM operator|(ENUM, ENUM)
//!  - ENUM& operator&=(ENUM&, ENUM)
//!  - ENUM operator&(ENUM, ENUM)
//!  - ENUM& operator^=(ENUM&, ENUM)
//!  - ENUM operator^(ENUM, ENUM)
//!  - ENUM operator~(ENUM)
//!
//! @pre ENUM must be an enum class type.
#define MTK_DEFINE_FLAG_OPERATORS(ENUM) \
	static_assert(std::is_enum_v<ENUM>, "ENUM must be an enum class type"); \
	static_assert(!std::is_convertible_v<ENUM, std::underlying_type_t<ENUM>>, "ENUM must be an enum class type"); \
	MTK_IMPL_FLAG_OPERATOR(ENUM, |) \
	MTK_IMPL_FLAG_OPERATOR(ENUM, &) \
	MTK_IMPL_FLAG_OPERATOR(ENUM, ^) \
	constexpr ENUM operator~(ENUM rhs) noexcept \
	{ \
		using _ut = std::underlying_type_t<ENUM>; \
		return static_cast<ENUM>(~static_cast<_ut>(rhs)); \
	}

//! @}

} // namespace mtk

#endif
