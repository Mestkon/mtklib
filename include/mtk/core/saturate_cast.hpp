#ifndef MTK_CORE_SATURATE_CAST_HPP
#define MTK_CORE_SATURATE_CAST_HPP

#include <mtk/core/types.hpp>
#include <mtk/core/impl/clamp.hpp>
#include <mtk/core/impl/require.hpp>

#include <limits>
#include <type_traits>

namespace mtk {
inline namespace casts {

template<class U
	,class T
#ifndef MTK_DOXYGEN
	,_require<std::is_arithmetic_v<U>> = 0
	,_require<std::is_arithmetic_v<T>> = 0
#endif
	>
constexpr
U
saturate_cast(T val) noexcept
{
	constexpr T t_min = std::numeric_limits<T>::lowest();
	constexpr T t_max = std::numeric_limits<T>::max();
	constexpr U u_min = std::numeric_limits<U>::lowest();
	constexpr U u_max = std::numeric_limits<U>::max();

	constexpr ldouble t_min_wide = static_cast<ldouble>(t_min);
	constexpr ldouble t_max_wide = static_cast<ldouble>(t_max);
	constexpr ldouble u_min_wide = static_cast<ldouble>(u_min);
	constexpr ldouble u_max_wide = static_cast<ldouble>(u_max);

	constexpr bool t_has_nan = std::numeric_limits<T>::has_quiet_NaN
		|| std::numeric_limits<T>::has_signaling_NaN;
	constexpr bool u_has_nan = std::numeric_limits<U>::has_quiet_NaN
		|| std::numeric_limits<U>::has_signaling_NaN;

	if constexpr (t_has_nan && !u_has_nan) {
		if (val != val) {
			return U();
		}
	}

	constexpr bool t_in_domain_of_u = (t_min_wide >= u_min_wide) && (t_max_wide <= u_max_wide);
	constexpr bool infinity_compatible = (std::numeric_limits<T>::has_infinity && std::numeric_limits<U>::has_infinity) ||
		!std::numeric_limits<T>::has_infinity;

	if constexpr (t_in_domain_of_u && infinity_compatible) {
		return static_cast<U>(val);
	} else {
		constexpr T min = (t_min_wide < u_min_wide ? static_cast<T>(u_min) : t_min);
		constexpr T max = (t_max_wide > u_max_wide ? static_cast<T>(u_max) : t_max);

		return static_cast<U>(mtk::_clamp(val, min, max));
	}
}

} // namespace casts
} // namespace mtk

#endif
